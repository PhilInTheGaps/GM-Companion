echo "Building online installer using QtInstallerFramework v3.2 ..."
echo "%QT_TOOLS%\QtInstallerFramework\3.2\bin\binarycreator.exe -n -t %QT_TOOLS%\QtInstallerFramework\3.2\bin\installerbase.exe -p packages -c config\config.xml gm-companion-online-installer-x64.exe"

%QT_TOOLS%\QtInstallerFramework\3.2\bin\binarycreator.exe -n -t %QT_TOOLS%\QtInstallerFramework\3.2\bin\installerbase.exe -p packages -c config\config.xml gm-companion-online-installer-x64.exe