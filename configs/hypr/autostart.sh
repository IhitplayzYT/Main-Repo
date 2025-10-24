
#!/bin/bash
sleep 1

kitty --class ws1-term -e bash -ic "clear; exec bash" &
flatpak run app.zen_browser.zen &
kitty --class ws3-kew -e bash -ic "kew; exec bash" &
kitty --class ws3-cava -e bash -ic "cava; exec bash" &
kitty --class ws3-btop -e bash -ic "btop; exec bash" &
kitty --class ws4-rain -e bash -ic "rain; exec bash" &
kitty --class ws4-cbonsai -e bash -ic "cbonsai; exec bash" &
kitty --class ws4-pipes -e bash -ic "pipes.sh; exec bash" &

