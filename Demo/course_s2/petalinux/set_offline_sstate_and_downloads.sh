#!/bin/sh

SSTATE_DEFAULT_PATH="http://petalinux.xilinx.com/sswreleases/rel-v${PETALINUX_MAJOR_VER}/aarch64/sstate-cache"
DOWNLOADS_DEFAULT_PATH="http://petalinux.xilinx.com/sswreleases/rel-v${PETALINUX_MAJOR_VER}/downloads"

echo -e "\e[1;33m1\e[0m\e[33m Set offline packages\e[0m"
echo -e "\e[1;33m2\e[0m\e[33m Reset offline packages\e[0m"
echo -e "\e[1;33mExit with any other value\e[0m"
echo -e -n "\e[33mWhat would you do\e[0m\e[5;33m:\e[0m"
read OPERATE
if [[ $OPERATE -eq 1 ]];then
	echo -e "\e[33mEnter the path where your local\e[0m \e[1;34msstate\e[0m \e[33mpackage is located."
	echo -e -n "\e[5;33m:\e[0m"
	read SSTATE_LOCAL_PATH
	if [[ $SSTATE_LOCAL_PATH == "" ]];then
		SSTATE_LOCAL_PATH=$SSTATE_DEFAULT_PATH
	fi

	echo -e "\e[33mEnter the path where your local\e[0m \e[1;34mdownloads\e[0m \e[33mpackage is located."
	echo -e -n "\e[5;33m:\e[0m"
	read DOWNLOADS_LOCAL_PATH
	if [[ $DOWNLOADS_LOCAL_PATH == "" ]];then
		DOWNLOADS_LOCAL_PATH=$DOWNLOADS_DEFAULT_PATH
	fi
	
	SSTATE_PATH_TOSET=$SSTATE_LOCAL_PATH
	DOWNLOADS_PATH_TOSET=$DOWNLOADS_LOCAL_PATH

	echo -e "\e[1;33m$SSTATE_PATH_TOSET\e[0m \e[33mwill set to sstate path\e[0m"
	echo -e "\e[1;33m$DOWNLOADS_PATH_TOSET\e[0m \e[33mwill set to downloads path\e[0m"
elif [[ $OPERATE -eq 2 ]];then
	SSTATE_PATH_TOSET=$SSTATE_DEFAULT_PATH
	DOWNLOADS_PATH_TOSET=$DOWNLOADS_DEFAULT_PATH
	echo -e "\e[1;33mOffline packages will be reset, petalinux-build will get resources online.\e[0m"
else
	exit
fi

DOWNLOADS_LINE=`grep -nr CONFIG_PRE_MIRROR_URL ./project-spec/configs/config`
DOWNLOADS_LINE=${DOWNLOADS_LINE%%:*}

SSTATE_LINE=`grep -nr CONFIG_YOCTO_LOCAL_SSTATE_FEEDS_URL ./project-spec/configs/config`
SSTATE_LINE=${SSTATE_LINE%%:*}

sed -i "${SSTATE_LINE}s#.*#CONFIG_YOCTO_LOCAL_SSTATE_FEEDS_URL=\"${SSTATE_PATH_TOSET}\"#" ./project-spec/configs/config
sed -i "${DOWNLOADS_LINE}s#.*#CONFIG_PRE_MIRROR_URL=\"${DOWNLOADS_PATH_TOSET}\"#" ./project-spec/configs/config

petalinux-config --silentconfig
echo -e "\e[1;32moffline package set over\e[0m."




