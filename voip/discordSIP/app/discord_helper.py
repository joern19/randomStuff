import discord
import os
import time

class DiscordBotClient(discord.Client):
    async def on_ready(self):
        print("ready")

    async def on_message(self, message):
        voice_channel = message.author.voice.channel
        print(voice_channel)
        voice_client: discord.VoiceClient = await voice_channel.connect()
        print(voice_client)

        voice_client.play(discord.FFmpegPCMAudio(executable="/usr/bin/ffmpeg", source="/home/not-a-robot/Downloadable/test.wav"))

        time.sleep(10)
        await voice_client.disconnect()

client = DiscordBotClient(intents=discord.Intents.default())
client.run(os.environ.get("DISCORD_BOT_TOKEN"))

