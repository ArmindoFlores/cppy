import os
import subprocess

import setuptools
import setuptools.command.build_py


class CustomBuild(setuptools.command.build_py.build_py):
    def run(self):
        if not all([
            os.path.isfile(os.path.join("cppy", "Python3Lexer.py")),
            os.path.isfile(os.path.join("cppy", "Python3Parser.py")),
            os.path.isfile(os.path.join("cppy", "Python3Visitor.py"))
        ]):
            print("Trying to generate the parser, lexer and visitor")
            process = subprocess.run(["java", "org.antlr.v4.Tool", "-Dlanguage=Python3", "-visitor", "-no-listener", os.path.join("cppy", "Python3.g4")])
            if process.returncode != 0:
                process = subprocess.run(["antlr4", "-Dlanguage=Python3", "-visitor", "-no-listener", os.path.join("cppy", "Python3.g4")])
                if process.returncode != 0:
                    print("Failed to run antlr4")

        else:
            print("Found parser, lexer and visitor")
        super().run()

    
with open("requirements.txt", "r") as fh:
    requires = [line for line in fh.read().splitlines() if line != ""]

setuptools.setup(
    cmdclass={"build_py": CustomBuild},
    name="cppy",
    version="0.0.1",
    author="Francisco Rodrigues",
    author_email="francisco.rodrigues0908@gmail.com",
    description="A Python to C++ transpiler",
    python_requires='>=3.8',
    install_requires=requires,
    long_description_content_type="text/markdown",
    url="https://github.com/ArmindoFlores/cppy",
    packages=setuptools.find_packages(),
    classifiers=[
        "Programming Language :: Python :: 3",
        "License :: OSI Approved :: MIT License",
        "Operating System :: OS Independent",
    ],
)
