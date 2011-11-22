cd %~dp0
@rem convert_net_snmp_form_md_to_mt
for /R .\deps\net-snmp\win32 %%i IN (*Makefile*.in) DO copy /Y %%i %%i.old && sed -e "s/ \/MD / \/MT /" %%i.old >%%i && copy /Y %%i %%i.old && sed -e "s/ \/MDd / \/MTd /" %%i.old >%%i  && del %%i.old
set PATH=%PATH%;c:\perl\bin
cd .\deps\net-snmp\win32\
call build.bat
cd %~dp0