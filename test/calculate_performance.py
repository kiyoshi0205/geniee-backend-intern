import argparse
from concurrent.futures import ThreadPoolExecutor
import json
import os
import queue
import random
import string
import subprocess
import threading
import time

import requests

from sample_data import *

SCRIPT_ROOT = os.path.abspath(
    os.path.join(os.path.dirname(os.path.abspath(__file__)), os.pardir)
)


def random_string(length: int) -> str:
    return "".join(random.choices(string.ascii_letters + string.digits, k=length))


def get_user() -> str:
    return (
        subprocess.run(
            ["make", "-C", SCRIPT_ROOT, "get-submit-user"],
            capture_output=True,
            text=True,
        )
        .stdout.strip()
        .split("\n")[1]
    )


def get_language() -> str:
    language = (
        subprocess.run(
            ["make", "-C", SCRIPT_ROOT, "get-language"], capture_output=True, text=True
        )
        .stdout.strip()
        .split("\n")[1]
    )
    mp = {
        "python": "Python",
        "golang": "Golang",
        "cpp": "C++",
    }
    return mp[language]


BASE_URL = "http://34.57.193.114/api/"


def register_for_dashboard(dataset) -> str:
    try:
        username = get_user()
        language = get_language()
        dataset = dataset[0].upper() + dataset[1:]
        language = language[0].upper() + language[1:]
        data = {"name": username, "dataset": dataset, "language": language}
        res = requests.post(BASE_URL + "register", json=data, timeout=1)
        return res.json()["token"]
    except:
        print("Failed to register for dashboard. continue...")


def submit_for_dashboard(token, elapsed_time, hash) -> None:
    data = {"token": token, "time": elapsed_time, "hash": hash}
    try:
        requests.post(BASE_URL + "submit", json=data, timeout=1)
    except:
        print("\nFailed to submit for dashboard. continue...")


N_REQUEST = 10
N_WORKER = 10
BAR_WIDTH = 20


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="seed値を受け取りリクエストを送信する")
    parser.add_argument(
        "seed", help="乱数のシード。シードが一致していれば送信されるデータは同一"
    )
    parser.add_argument(
        "datasize",
        choices=["small", "medium", "large", "challenge"],
        help="検証するリクエスト規模",
    )
    parser.add_argument(
        "--skip-validation",
        action="store_true",
        help="スピードチェックのみ行う。レスポンスの正当性は確認しない",
    )
    parser.add_argument(
        "--submit", action="store_true", help="外部サーバに本番データを提出する"
    )
    args = parser.parse_args()
    datasize = args.datasize
    seed = args.seed
    skip_validation = args.skip_validation
    submit = args.submit

    random.seed(seed)
    coff = 1
    prohibit_domain_num = 1
    threshold = 0
    if datasize == "small":
        coff = 1
        prohibit_domain_num = 1
        threshold = 2
    elif datasize == "medium":
        coff = 10
        prohibit_domain_num = 3
        threshold = 5
    elif datasize == "large":
        coff = 50
        prohibit_domain_num = 5
        threshold = 5
    elif datasize == "challenge":
        coff = 500
        prohibit_domain_num = 10
        threshold = 10
    else:
        raise ValueError("invalid datasize")

    token = register_for_dashboard(datasize) if submit else ""

    que = queue.Queue()
    for _ in range(N_REQUEST * coff):
        n_category_blockings = random.randint(1, 10)
        category_ids = random.sample(range(1, 21), k=n_category_blockings)
        zone_feature = random.randint(-100000, 100000)
        zone_width, zone_height = random.choice(CANDIDATE_ZONE_SIZES)
        block_domains = random.sample(
            PROHIBIT_STRINGS, k=min(len(PROHIBIT_STRINGS), prohibit_domain_num))

        request_data = {
            "auction_id": random_string(16),
            "category_ids": category_ids,
            "zone_feature": zone_feature,
            "zone_width": zone_width,
            "zone_height": zone_height,
            "block_domains": block_domains,
        }
        res = json.dumps(request_data)
        que.put(res)

    response = []
    done_threads = 0
    server_alive = True
    lock = threading.Lock()
    start_time = time.time()

    def progress_bar():
        while True:
            count = len(response)
            processed = count * BAR_WIDTH // (N_REQUEST * coff)
            eps_time = time.time() - start_time
            print(
                "\r[{}] {}/{} ({} req/sec)".format(
                    "#" * processed + "-" * (BAR_WIDTH - processed),
                    count,
                    N_REQUEST * coff,
                    round(count / eps_time, 2),
                ),
                end="",
            )
            if count >= N_REQUEST * coff or done_threads >= N_WORKER:
                break
            time.sleep(0.1)
            if not server_alive:
                print(
                    "\nServer down detected. Check your log files (test/stdout.log, test/stderr.log)"
                )
                time.sleep(0.9)

    def send_request() -> None:
        global done_threads
        global server_alive
        while True:
            try:
                data = que.get(timeout=0.1)
            except:
                with lock:
                    done_threads += 1
                break

            try:
                r = requests.post("http://localhost:6562/auction", data=data)
                server_alive = True
            except:
                que.put(data)
                server_alive = False
                continue

            if r.status_code == 200:
                response.append(r.text)
            elif r.status_code == 204:
                failed_auction_id = json.loads(data).get("auction_id")
                response.append(
                    json.dumps(
                        {"auction_id": failed_auction_id, "message": "No Contents"}
                    )
                )
            else:
                que.put(data)

    with ThreadPoolExecutor(max_workers=N_WORKER + 1) as executor:
        for _ in range(N_WORKER):
            executor.submit(send_request)
        executor.submit(progress_bar)
    elapsed_time = round(time.time() - start_time, 4)

    response.sort()
    with open(os.path.join(SCRIPT_ROOT, "test/result.json"), mode="w") as f:
        f.write("\n".join(response) + "\n")

    def get_md5sum(filename: str) -> str:
        md5sum = subprocess.run(["md5sum", filename],
                                capture_output=True, text=True)
        return md5sum.stdout.split(" ")[0]

    result_md5 = get_md5sum(os.path.join(SCRIPT_ROOT, "test/result.json"))
    ans_md5 = get_md5sum(
        os.path.join(SCRIPT_ROOT, f"data/response_{seed}_{datasize}.json")
    )

    if submit:
        submit_for_dashboard(token, int(elapsed_time * 1000), result_md5)
        # submitする時はローカルで正当性チェックは不可能。時間だけ見る。
        print(f"\nresponse check HIDDEN (see dashboard)")
        print(f"elapsed time: {elapsed_time} sec. ", end="")
        if elapsed_time >= threshold:
            print("performance check Time Limit Exceeded...")
            exit(1)
        else:
            print("performance check ACCEPTED")
    else:
        if skip_validation or result_md5 == ans_md5:
            if skip_validation:
                print("\nresponse check SKIPPED")
            else:
                print("\nresponse check PASSED")
            print(f"elapsed time: {elapsed_time} sec. ", end="")
            if elapsed_time >= threshold:
                print("performance check Time Limit Exceeded...")
                exit(1)
            else:
                print("performance check ACCEPTED")
        else:
            print("\nresponse check FAILED...")
            exit(1)
