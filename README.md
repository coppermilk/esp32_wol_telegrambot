# ESP32 WakeOnLan: remote computer start
For a quick and reliable way to turn on a computer from anywhere, without the need to forward broadcast packets or maintain local servers, a [simple Telegram bot on an ESP32.](https://github.com/coppermilk/esp32_wol_telegrambot)

To create such a bot, you need to use an ESP32 dev board, which can connect to a local network via Wi-Fi and consumes very little power — less than half a watt.

For ease of use, you can use the Arduino Framework.

The code for the bot should be simple and clear and can be found on Github. To compile, you need to install additional libraries:

-   [WakeOnLan](https://www.arduino.cc/reference/en/libraries/wakeonlan/)
-   [UniversalTelegramBot](https://www.arduino.cc/reference/en/libraries/universaltelegrambot/)

To set up the bot, you need to fill in the Wi-Fi configuration and MAC address of the target device, as well as the Telegram user token and ID, so that no one else can use the bot.

![](https://miro.medium.com/max/350/1*FxVnO1-fXCenr2hSMueKyA.png)

The code also revised the reboot of the board once every 4 hours so that the board does not freeze. And using the Telegram API every 5 seconds.

![](https://miro.medium.com/max/350/1*vnB6D7ttFj5Anw6JxAEYEw.png)

Using the bot is very simple — just send the _/start_ command to get a list of available commands and use the _/wol_ command.
