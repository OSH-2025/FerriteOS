# Installing the Compiler and Builder<a name="EN-US_TOPIC_0314196089"></a>

-   Install the GNU Arm Embedded Toolchain compiler.
    1.  Download the compiler.

        For a 32-bit chip architecture, you are advised to use the  [GNU Arm Embedded Toolchain 2019-q4-major or a later version](https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/gnu-rm/downloads).

        For a 64-bit chip architecture, you are advised to use the  [latest aarch64-linux-gnu GNU Arm Embedded Toolchain](https://www.linaro.org/downloads/).

    2.  Decompress the compiler.

        Run the following command to decompress the package. Replace the package name with the downloaded software package name.

        ```
        tar -xvf gcc-arm-none-eabi-9-2019-q4-major-x86_64-linux.tar.bz2
        ```

        After decompression, the  **gcc-arm-none-eabi-9-2019-q4-major**  folder is generated.

    3.  Add the compiler execution path to the environment variable.

        Use the compiler decompressed in step 2 as an example. Add the  **gcc-arm-none-eabi-9-2019-q4-major/bin**  directory to the  _PATH_  environment variable and edit the  **\~/.bashrc**  file.

        ```
        export PATH=$PATH:YOUR_PATH/gcc-arm-none-eabi-9-2019-q4-major/bin/
        ```

        Run the following command to make the new environment variable take effect:

        ```
        source ~/.bashrc
        ```


-   Upgrade the GNU Make builder to the latest version.
    1.  Download the latest Make builder from the  [official website](http://ftp.gnu.org/pub/gnu/make/?C=M;O=D).
    2.  Run the following command to decompress the package. Replace the package name with the downloaded software package name.

        ```
        tar -xf make-4.3.tar.gz
        ```

    3.  Check the dependency.

        After the package is decompressed, go to the directory, and run the  **./configure**  command to check the dependency required for build and installing the Make builder.

        ```
        cd make-4.3
        ./configure
        ```

        If no error is reported, go to the next step. If an error is reported, install the dependency as prompted.

    4.  Build and install the Make builder.

        In the current directory, run the following commands to build and install the Make builder:

        ```
        sh build.sh
        sudo make
        sudo make install
        ```



