#include "WiFiMulti.h"
#include "WiFiClientSecure.h"
#include "WiFiUDP.h"
#include "WakeOnLan.h" // https://www.arduino.cc/reference/en/libraries/wakeonlan/
#include "UniversalTelegramBot.h" // https://www.arduino.cc/reference/en/libraries/universaltelegrambot/
#include "ArduinoJson.h"

// Telegram Bot Token
#define BOT_TOKEN "0000000000:000000000000000000000000000000000000"
#define ALLOWED_ID "000000000"

// WiFi configuration
#define WIFI_SSID "<wifi-network-name>"
#define WIFI_PASS "<wifi-password>"

// MAC address of the target device
#define MAC_ADDR "00:00:00:00:00:00"

const unsigned long BOT_MTBS = 5000;	// Mean time between scan messages in microsecond.
const unsigned long RESTRART_MTBS = 1000 * 3600 * 4; // Means restart every 4 hours.

WiFiMulti wifiMulti;
WiFiClientSecure secured_client;
WiFiUDP UDP;
WakeOnLan WOL(UDP);

UniversalTelegramBot bot(BOT_TOKEN, secured_client);
unsigned long bot_lasttime;	// last time messages' scan has been done

void sendWOL()
{
	WOL.sendMagicPacket(MAC_ADDR);	// send WOL on default port (9)
	delay(300);
}

void handleNewMessages(int numNewMessages)
{
	Serial.print("handleNewMessages ");
	Serial.println(numNewMessages);

	for (int i = 0; i < numNewMessages; ++i)
	{
		Serial.println(bot.messages[i].from_id);
		if (bot.messages[i].from_id != ALLOWED_ID) 
    {
      continue;
    }

		String chat_id = bot.messages[i].chat_id;
		String text = bot.messages[i].text;
		String from_name = bot.messages[i].from_name;

		if (from_name == "")
    {
      from_name = "Guest";
    }

		if (text == "/wol")
		{
			sendWOL();
			bot.sendMessage(chat_id, "Magic Packet sent!", "");
		}
		else
		{
			String welcome = "Welcome to **WoL Bot**, " + from_name + ".\n";
			welcome += "/wol : Send the Magic Packet\n";
			bot.sendMessage(chat_id, welcome, "Markdown");
		}
	}
}

void setup()
{
	Serial.begin(9600);
	// Configure WiFI
	wifiMulti.addAP(WIFI_SSID, WIFI_PASS);

	// Add root certificate for api.telegram.org
	secured_client.setCACert(TELEGRAM_CERTIFICATE_ROOT);

	Serial.print("Connecting to WiFI...");
	while ((wifiMulti.run() != WL_CONNECTED))
	{
		// wait for WiFi connection.
		delay(500);
		Serial.print(".");
	}

	Serial.println("OK");

	// Attention: 255.255.255.255 is denied in some networks
	WOL.calculateBroadcastAddress(WiFi.localIP(), WiFi.subnetMask());

	Serial.print("Retrieving time...");
	configTime(0, 0, "pool.ntp.org");	// get UTC time via NTP
	time_t now = time(nullptr);
	while (now < 24 *3600)
	{
		Serial.print(".");
		delay(150);
		now = time(nullptr);
	}

	Serial.println(now);
}

void loop()
{
	if (millis() - bot_lasttime > BOT_MTBS)
	{
		int numNewMessages = bot.getUpdates(bot.last_message_received + 1);
		while (numNewMessages)
		{
			Serial.println("Response received");
			handleNewMessages(numNewMessages);
			numNewMessages = bot.getUpdates(bot.last_message_received + 1);
		}

		bot_lasttime = millis();
	}

  // Restart.
	if (millis() > RESTRART_MTBS)
  {
    Serial.println("Restart\n");
    ESP.restart();
  }
  delay(100);
}
