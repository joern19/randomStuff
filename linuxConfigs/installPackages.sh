if ! [ "$EUID" -ne 0 ]
  then echo "Please do not run as root"
  exit
fi

sudo pacman -Syu
sudo pacman -S i3 playerctl feh dmenu ttf-font-awesome spotifyd rxvt-unicode xsel xfce4-terminal xcompmgr
sudo snap install spt

mkdir -p ~/.config/autostart
echo "
[Desktop Entry]
Encoding=UTF-8
Type=Application
Name=xcompmgr
Exec=xcompmgr
StartupNotify=true" > ~/.config/autostart/xcompmgr.desktop

# Install font if not installed already
FILE="~/.fonts/System\ San\ Francisco\ Display\ Bold.ttf"
if ! [[ -f "$FILE" ]]; then
  wget https://github.com/supermarin/YosemiteSanFranciscoFont/archive/master.zip -O tmpFile.zip
  unzip tmpFile.zip
  rm tmpFile.zip
  mv YosemiteSanFranciscoFont-master/*.ttf ~/.fonts/
  rm -r YosemiteSanFranciscoFont-master/
fi


mkdir -p ~/.urxvt/ext
wget https://raw.githubusercontent.com/muennich/urxvt-perls/master/keyboard-select -O ~/.urxvt/ext/keyboard-select
