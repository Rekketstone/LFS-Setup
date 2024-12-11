#! /bin/bash

function powermenu {
  options="Cancel\nShutdown\nRestart\nSleep\nLock\n..."
  selected=$(echo -e $options | dmenu -c -l 5 -nb '#000000' -nf '#ffffff' -sb '#ffffff' -sf '#000000' -bw 2)
  if [[ $selected = "Shutdown" ]]; then
    loginctl poweroff
  elif [[ $selected = "Restart" ]]; then
    loginctl reboot
  elif [[ $selected = "Sleep" ]]; then
    loginctl suspend
  elif [[ $selected = ""Lock"" ]]; then
    slock -m "$(cowsay -f /usr/local/share/cowsay-files/cows/melisandre.cow \"SUCKLESS: THE EPITOME OF SOFTWARE\!\")"
  elif [[ $selected = "..." ]]; then
    xdg-open 'https://duckduckgo.com/?q=Am+I+Gay+if+I+like+a+femboy'
  elif [[ $selected = "Cancel" ]]; then
    return
  fi
}

powermenu
