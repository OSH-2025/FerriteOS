# Installing the Graphical Configuration Parser<a name="EN-US_TOPIC_0314196090"></a>

-   Install Python 2.7/3.2+.

    The following uses Python 3.8 as an example to describe the installation method.

    -   Installation through CLI:

        ```
        sudo apt-get install python3.8
        ```

    -   Installation through source code compilation:
        1.  Download the  [official Python source code package](https://www.python.org/ftp/python/3.8.5/Python-3.8.5.tgz).
        2.  Decompress the source code package.

            Run the following command to decompress the package. Replace the package name with the downloaded source code package name.

            ```
            tar -xf Python-3.8.5.tgz
            ```

        3.  Check the dependency.

            After the package is decompressed, go to the directory, and run the  **./configure**  command to check the dependency required for compiling and installing Python.

            ```
            cd Python-3.8.5
            ./configure
            ```

            If no error is reported, go to the next step. If an error is reported, install the dependency as prompted.

        4.  Compile and install Python.

            ```
            sudo make
            sudo make install
            ```

        5.  Check the Python version and properly link the Python command.

            ```
            python --version
            ```

            If the displayed version is not the installed Python version, run the following command to properly link the Python command:

            1.  Obtain the Python directory. For example, for Python 3.8.5, run the following command:

                ```
                which python3.8
                ```

            2.  Link the Python command to the newly installed Python package.

                Replace  **python3.8-path**  in the following command with the path displayed after the  **which python3.8**  command is executed:

                ```
                cd /usr/bin && sudo rm python && sudo ln -s "python3.8-path" python
                ```

            3.  Check the Python version again.

                ```
                python --version
                ```




-   Install the pip package management tool.

    If no pip command is available, download the pip source code package and install it. pip depends on setuptools. Install pip even if setuptools does not exist.

    -   Installation through CLI:

        ```
        sudo apt-get install python3-setuptools python3-pip -y
        sudo pip3 install --upgrade pip
        ```

    -   Installation through the source code package:
        1.  Install setuptools.

            Click the  [download address of the setuptools source code package](https://pypi.org/project/setuptools/)  and run the following commands to install setuptools:

            ```
            sudo unzip setuptools-50.3.2.zip
            cd setuptools
            sudo python setup.py install
            ```

            >![](public_sys-resources/icon-notice.gif) **NOTICE:** 
            >
            >The latest setuptools version does not support Python 2.7. To use Python 2.7, download  [setuptools 45.0.0](https://pypi.org/project/setuptools/45.0.0/#files).

        2.  Install pip.

            Click the  [download address of the pip source code package](https://pypi.org/project/pip/)  and run the following commands to install pip:

            ```
            sudo tar -xf pip-20.2.4.tar.gz
            cd pip-20.2.4
            sudo python setup.py install
            ```



-   Install the kconfiglib library.
    -   If the server can be connected to the Internet:

        Run the following command to install kconfiglib:

        ```
        sudo pip install kconfiglib
        ```

    -   If the server cannot be connected to the Internet:

        Install kconfiglib in offline mode. Download  [kconfiglib](https://pypi.org/project/kconfiglib/#files)  from an environment that can be connected to the Internet. You can download either the wheel file  **kconfiglib-14.1.0-py2.py3-none-any.whl**  or the source code file  **kconfiglib-14.1.0.tar.gz**. The following uses kconfiglib 14.1.0 as an example.

        -   To install the wheel file, run the following command:

            ```
            sudo pip install kconfiglib-14.1.0-py2.py3-none-any.whl
            ```

        -   To install the source code file, run the following commands:

            ```
            sudo tar -zxvf kconfiglib-14.1.0.tar.gz
            cd kconfiglib-14.1.0
            sudo python setup.py install
            ```




