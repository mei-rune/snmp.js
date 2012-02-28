cd %~dp0
@rem convert_net_snmp_form_md_to_mt
for /R .\deps\net-snmp\win32 %%i IN (*Makefile*.in) DO copy /Y %%i %%i.old && sed -e "s/ \/MD / \/MT /" %%i.old >%%i && copy /Y %%i %%i.old && sed -e "s/ \/MDd / \/MTd /" %%i.old >%%i  && del %%i.old


for /R .\deps\net-snmp\snmplib\openssl %%i IN (openssl_*.?) DO if not exist %%i.old copy /Y %%i %%i.old
for /R .\deps\net-snmp\snmplib\openssl %%i IN (openssl_*.?) DO sed -e "s/#include \"openssl_des_local\.h\"/#include <net-snmp\/net-snmp-config\.h>\n#include \"openssl_des_local\.h\"/" %%i.old > %%i


set PATH=%PATH%;c:\perl\bin
cd .\deps\net-snmp\win32\



@rem add #define NETSNMP_USE_INTERNAL_CRYPTO 1 after #define NETSNMP_USE_INTERNAL_MD5 1
if not exist net-snmp\net-snmp-config.h.in.old copy net-snmp\net-snmp-config.h.in net-snmp\net-snmp-config.h.in.old
sed -e "s/#define NETSNMP_USE_INTERNAL_MD5 1/#define NETSNMP_USE_INTERNAL_MD5 1\n#define NETSNMP_USE_INTERNAL_CRYPTO 1\n#define OPENSSL_SYS_WIN32 1\n#define HAVE_AES 1\n/" net-snmp\net-snmp-config.h.in.old > net-snmp\net-snmp-config.h.in


@rem add openssl\openssl_*.c
IF NOT EXIST libsnmp\Makefile.in.olda. copy libsnmp\Makefile.in libsnmp\Makefile.in.olda
sed -e "s/\/Fd\"$(INTDIR)\\\\$(PROGNAME)\.pdb\"/\/Fd\"..\\\\lib\\\\$(OUTDIR)\\\\netsnmp.pdb\"/" libsnmp\Makefile.in.olda > libsnmp\Makefile.in.new
sed -e "s/\"$(INTDIR)\\winpipe.obj\"$/\"$(INTDIR)\\winpipe.obj\" \\\n  \"$(INTDIR)\\openssl_aes_cfb\.obj\" \\\n  \"$(INTDIR)\\openssl_aes_core.obj\" \\\n  \"$(INTDIR)\\openssl_cbc_enc\.obj\" \\\n  \"$(INTDIR)\\openssl_cfb128\.obj\" \\\n  \"$(INTDIR)\\openssl_des_enc\.obj\" \\\n  \"$(INTDIR)\\openssl_md5\.obj\" \\\n  \"$(INTDIR)\\openssl_set_key\.obj\" \\\n  \"$(INTDIR)\\openssl_sha1\.obj\" \n/" libsnmp\Makefile.in.new > libsnmp\Makefile.in

echo.>> libsnmp\Makefile.in
echo SOURCE=..\..\snmplib\openssl\openssl_aes_cfb.c >> libsnmp\Makefile.in
echo.>> libsnmp\Makefile.in
echo "$(INTDIR)\openssl_aes_cfb.obj" : $(SOURCE) "$(INTDIR)"  >> libsnmp\Makefile.in
echo.	$(CPP) $(CPP_PROJ) $(SOURCE) >> libsnmp\Makefile.in
echo.>> libsnmp\Makefile.in
	
echo SOURCE=..\..\snmplib\openssl\openssl_aes_core.c >> libsnmp\Makefile.in
echo.>> libsnmp\Makefile.in
echo "$(INTDIR)\openssl_aes_core.obj" : $(SOURCE) "$(INTDIR)"  >> libsnmp\Makefile.in
echo.	$(CPP) $(CPP_PROJ) $(SOURCE) >> libsnmp\Makefile.in
echo.>> libsnmp\Makefile.in

echo SOURCE=..\..\snmplib\openssl\openssl_cbc_enc.c >> libsnmp\Makefile.in
echo "$(INTDIR)\openssl_cbc_enc.obj" : $(SOURCE) "$(INTDIR)"  >> libsnmp\Makefile.in
echo.	$(CPP) $(CPP_PROJ) $(SOURCE) >> libsnmp\Makefile.in
echo.

echo SOURCE=..\..\snmplib\openssl\openssl_cfb128.c >> libsnmp\Makefile.in
echo.>> libsnmp\Makefile.in
echo "$(INTDIR)\openssl_cfb128.obj" : $(SOURCE) "$(INTDIR)"  >> libsnmp\Makefile.in
echo.	$(CPP) $(CPP_PROJ) $(SOURCE) >> libsnmp\Makefile.in
echo.>> libsnmp\Makefile.in

echo SOURCE=..\..\snmplib\openssl\openssl_des_enc.c >> libsnmp\Makefile.in
echo.>> libsnmp\Makefile.in
echo "$(INTDIR)\openssl_des_enc.obj" : $(SOURCE) "$(INTDIR)"  >> libsnmp\Makefile.in
echo.	$(CPP) $(CPP_PROJ) $(SOURCE) >> libsnmp\Makefile.in
echo.>> libsnmp\Makefile.in

echo SOURCE=..\..\snmplib\openssl\openssl_md5.c >> libsnmp\Makefile.in
echo.>> libsnmp\Makefile.in
echo "$(INTDIR)\openssl_md5.obj" : $(SOURCE) "$(INTDIR)"  >> libsnmp\Makefile.in
echo.	$(CPP) $(CPP_PROJ) $(SOURCE) >> libsnmp\Makefile.in
echo.>> libsnmp\Makefile.in

echo SOURCE=..\..\snmplib\openssl\openssl_set_key.c >> libsnmp\Makefile.in
echo.>> libsnmp\Makefile.in
echo "$(INTDIR)\openssl_set_key.obj" : $(SOURCE) "$(INTDIR)"  >> libsnmp\Makefile.in
echo.	$(CPP) $(CPP_PROJ) $(SOURCE) >> libsnmp\Makefile.in
echo.>> libsnmp\Makefile.in

echo SOURCE=..\..\snmplib\openssl\openssl_sha1.c >> libsnmp\Makefile.in
echo.>> libsnmp\Makefile.in
echo "$(INTDIR)\openssl_sha1.obj" : $(SOURCE) "$(INTDIR)"  >> libsnmp\Makefile.in
echo.	$(CPP) $(CPP_PROJ) $(SOURCE) >> libsnmp\Makefile.in
echo.>> libsnmp\Makefile.in

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