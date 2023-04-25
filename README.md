# KAzeron
qmk_firmware/keyboard/KAzeron

qmk firmware for
https://www.instructables.com/Azeron-Game-Pad-DIY-Under-35/

使用Arduino promicro
接線圖與機構都跟來源一樣

QMK Firmware 0.20.4
qmk後續版本會改一些東西，可能會不會適用，建議使用同版本

支援VIA自定義鍵，使用VIA 3.0版本
設定檔
kazeron.json 簡易排版

* joystick支援四種功能，預設使用搖桿中鍵切換功能，國際循環檔
  * WASD (遊戲方向按鍵)
  * MOUSE (未調速，應該要自行調整deadzone使它效果更好)
  * SCROLL (使用qmk內建按鍵功能)
  * JOYSTICK (連上電腦後需使用windows內建搖桿校正)


