import subprocess
import getpass
import os

password = getpass.getpass()


def f(password):
    def get_remove_items(target_dir: str) -> list[str]:
        return subprocess.run(
            f"sudo -S ls {target_dir}",
            shell=True,
            check=True,
            input=password,
            encoding="utf-8",
            stdout=subprocess.PIPE,
        ).stdout.split("\n")

    print(get_remove_items(os.path.join(os.environ["HOME"], "Desktop")))


f(password)
