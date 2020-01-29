echo "Building offline installer using QtInstallerFramework v3.2 ..."
echo "%QT_TOOLS%\QtInstallerFramework\3.2\bin\binarycreator.exe --offline-only -t %QT_TOOLS%\QtInstallerFramework\3.2\bin\installerbase.exe -p packages -c config\config.xml gm-companion-installer-x64.exe"

%QT_TOOLS%\QtInstallerFramework\3.2\bin\binarycreator.exe --offline-only -t %QT_TOOLS%\QtInstallerFramework\3.2\bin\installerbase.exe -p packages -c config\config.xml gm-companion-installer-x64.exe