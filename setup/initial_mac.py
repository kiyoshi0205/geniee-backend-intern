import getpass
import os
import shlex
import subprocess
import sys

REPOSITORY_NAME = "geniee-backend-intern"
REPOSITORY_URL = "https://github.com/kiyoshi0205/geniee-backend-intern.git"


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


def destroy_local(password: str):
    INITIAL_DIRS = [
        ".Trash",
        "Applications",
        "Desktop",
        "Documents",
        "Downloads",
        "Library",
        "Movies",
        "Music",
        "Pictures",
        "Public",
    ]

    CAN_FORMAT_DIRS = [
        "Desktop",
        "Documents",
        "Downloads",
        "Movies",
        "Music",
        "Pictures",
    ]

    home_dir = os.environ["HOME"]
    for item in os.listdir(home_dir):
        if item and item not in INITIAL_DIRS:
            print(f'Removing "{item}" from {home_dir}')
            subprocess.run(
                "sudo -S rm -rf " + os.path.join(home_dir, shlex.quote(item)),
                shell=True,
                check=True,
                input=password.encode(),
            )

    def get_remove_items(target_dir: str) -> list[str]:
        return subprocess.run(
            "sudo -S ls " + target_dir,
            shell=True,
            check=True,
            input=password,
            encoding="utf-8",
            stdout=subprocess.PIPE,
        ).stdout.split("\n")

    for item in CAN_FORMAT_DIRS:
        target_dir = os.path.join(home_dir, item)
        for item in get_remove_items(target_dir):
            if item and item != ".localized":
                print(f'Removing "{item}" from {target_dir}')
                subprocess.run(
                    "sudo -S rm -rf " + os.path.join(target_dir, shlex.quote(item)),
                    shell=True,
                    check=True,
                    input=password.encode(),
                )


def install_homebrew(password: str):
    subprocess.run(
        "sudo -S rm -rf /opt/homebrew", shell=True, check=True, input=password.encode()
    )
    subprocess.run(
        '/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"',
        shell=True,
        check=True,
        input=password.encode(),
    )
    for file_name in [".bash_profile", ".zprofile", ".zshrc", ".bashrc"]:
        with open(file=os.path.join(os.environ["HOME"], file_name), mode="w") as f:
            f.write('eval "$(/opt/homebrew/bin/brew shellenv)"\n')


def install_git():
    subprocess.run("/opt/homebrew/bin/brew install git", shell=True, check=True)


def install_vscode(password: str):
    subprocess.run(
        "sudo -S rm -rf /Applications/Visual\\ Studio\\ Code.app",
        shell=True,
        check=True,
        input=password.encode(),
    )
    subprocess.run(
        "/opt/homebrew/bin/brew install --cask visual-studio-code && /opt/homebrew/bin/brew reinstall --cask visual-studio-code",
        shell=True,
        check=True,
    )

    def get_extension() -> list[str]:
        result = subprocess.run(
            "/opt/homebrew/bin/code --list-extensions",
            shell=True,
            check=True,
            stdout=subprocess.PIPE,
        ).stdout.decode()
        if result == "":
            return []
        return result.split("\n")

    # 依存関係の確認が面倒なので、一つずつ消していく
    while True:
        extensions = get_extension()
        if not extensions:
            break
        subprocess.run(
            f"/opt/homebrew/bin/code --uninstall-extension {extensions[0]}",
            shell=True,
            check=True,
        )


def install_docker(password: str):
    subprocess.run(
        "sudo -S rm -rf /Applications/Rancher\\ Desktop.app",
        shell=True,
        check=True,
        input=password.encode(),
    )
    subprocess.run(
        "/opt/homebrew/bin/brew install --cask rancher && /opt/homebrew/bin/brew reinstall --cask rancher",
        shell=True,
        check=True,
    )


def install_chrome(password: str):
    subprocess.run(
        "sudo -S rm -rf /Applications/Google\\ Chrome.app ~/Library/Application\\ Support/Google",
        shell=True,
        check=True,
        input=password.encode(),
    )
    subprocess.run(
        "/opt/homebrew/bin/brew install --cask google-chrome", shell=True, check=True
    )


def clone_repository():
    subprocess.run(
        f"cd ~ && /opt/homebrew/bin/git clone {REPOSITORY_URL} --recurse-submodules && cd ~/Desktop && ln -nfs ~/{REPOSITORY_NAME} {REPOSITORY_NAME}",
        shell=True,
        check=True,
    )


def install_language():
    subprocess.run(
        f"/opt/homebrew/bin/brew install gcc@12 go@1.22 python@3.12 && /opt/homebrew/bin/brew link go@1.22 && /opt/homebrew/opt/python@3.12/libexec/bin/pip3 install -r ~/{REPOSITORY_NAME}/python/requirements.txt --break-system-packages && /opt/homebrew/bin/code --install-extension ms-vscode.cpptools && /opt/homebrew/bin/code --install-extension golang.go && /opt/homebrew/bin/code --install-extension ms-python.python",
        shell=True,
        check=True,
    )


def main():
    password = get_sudo_password()
    if not check_sudo_password(password):
        print("Invalid password")
        sys.exit(1)
    destroy_local(password)
    install_homebrew(password)
    install_git()
    install_vscode(password)
    install_docker(password)
    install_chrome(password)
    clone_repository()
    install_language()


main()
