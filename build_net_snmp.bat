cd %~dp0
@rem convert_net_snmp_form_md_to_mt
for /R .\deps\net-snmp\win32 %%i IN (*Makefile*.in) DO copy /Y %%i %%i.old && sed -e "s/ \/MD / \/MT /" %%i.old >%%i && copy /Y %%i %%i.old && sed -e "s/ \/MDd / \/MTd /" %%i.old >%%i  && del %%i.old

set PATH=%PATH%;c:\perl\bin
cd .\deps\net-snmp\win32\
if not exist build.pl.old. copy build.pl build.pl.old
sed -e "s/ipv6 = \"disabled\"/ipv6 = \"enabled\"/" build.pl.old > build.pl.new
sed -e "s/winextdll = \"disabled\"/winextdll = \"enable\"/" build.pl.new > build.pl

sed -e "s/sdk = \"disabled\"/sdk = \"enabled\"/" build.pl > build.pl.new
sed -e "s/install_devel = \"disabled\"/install_devel = \"enable\"/" build.pl.new > build.pl

sed -e "s/debug = \"disabled\"/debug = \"enabled\"/" build.pl > build.pl.new

sed -e "s/logging = \"enabled\"/logging = \"disabled\"/" build.pl.new > build.pl
sed -e "s/while (1) {/if (1 eq 3) {/" build.pl > build.pl.new
copy /Y build.pl.new build.pl

call build.bat
cd %~dp0