import subprocess
import getpass
import os
import sys

print("version: 5")


def get_sudo_password() -> str:
    return getpass.getpass("Enter sudo password: ")


def check_sudo_password(password: str) -> bool:
    try:
        subprocess.run(
            "sudo -S ls",
            stdout=subprocess.DEVNULL,
            stderr=subprocess.DEVNULL,
            shell=True,
            check=True,
            input=password.encode(),
        )
        return True
    except subprocess.CalledProcessError:
        return False


def f(password: str):
    home_dir = os.environ["HOME"]

    def get_remove_items(target_dir: str) -> list[str]:
        print(list(map(int, password.encode())))
        print(list(map(int, target_dir.encode())))
        print(list(map(int, "/Users/geniee/Desktop".encode())))
        res = subprocess.run(
            "sudo -S ls " + target_dir,
            shell=True,
            check=True,
            input=password,
            encoding="utf-8",
            stdout=subprocess.PIPE,
        )
        print(res.returncode)
        return res.stdout.split("\n")

    print(get_remove_items(os.path.join(home_dir, "Desktop")))


def main():
    password = get_sudo_password()
    if not check_sudo_password(password):
        print("Invalid password")
        sys.exit(1)
    f(password)


main()
