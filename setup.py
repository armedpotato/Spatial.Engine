import os
import os.path
import sys


def run(command):
    print("\n\n > %s\n" % command)
    return os.system(command)


def conan_export(package_dir, export_name):
    return run("conan export %s %s" % (package_dir, export_name))


def install(folder):
    install_folder = os.path.abspath(folder)
    vendor_path = os.path.join(install_folder, "vendor")

    conan_export(os.path.join(vendor_path, "filament.py"), "filament/1.8.1@google/stable")
    conan_export(os.path.join(vendor_path, "entt.py"), "entt/3.4.0@skypjack/stable")
    conan_export(os.path.join(vendor_path, "imgui.py"), "imgui/docking@ocornut/stable")


if __name__ == "__main__":
    args = iter(sys.argv)
    next(args)  # skip file name

    root_directory = next(args, os.path.dirname(__file__))
    install(root_directory)
