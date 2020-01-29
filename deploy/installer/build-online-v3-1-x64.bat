echo "Building online installer using QtInstallerFramework v3.1 ..."
echo "%QT_TOOLS%\QtInstallerFramework\3.1\bin\binarycreator.exe -n -t %QT_TOOLS%\QtInstallerFramework\3.1\bin\installerbase.exe -p packages -c config\config.xml gm-companion-online-installer-x64.exe"

%QT_TOOLS%\QtInstallerFramework\3.1\bin\binarycreator.exe -n -t %QT_TOOLS%\QtInstallerFramework\3.1\bin\installerbase.exe -p packages -c config\config.xml gm-companion-online-installer-x64.exe