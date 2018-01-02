@echo off
echo 姓名=jiuhuo
echo 组织=www.jiuhuo.com
echo 公司=jiuhuogame
echo 城市=yt
echo 省份=sd
echo 国家=ch
keytool -genkey -alias jiuhuogame -keyalg RSA -keysize 1024 -keypass jiuhuogame -validity 365 -keystore Android.keystore -storepass jiuhuogame
pause
