from typing import Final, List
from telegram import Update
from telegram.ext import Application, CommandHandler, MessageHandler, filters, ContextTypes

class WeatherMonitering:
    token: Final = "7482602731:AAGMI9lG3mx-wWYfq9zdNRPi1T74qxOQilo"
    bot_username: Final = "@iot_wea_ther_bot"
    
    def __init__(self):
        self.chat_ids: List[int] = []
        self.app = Application.builder().token(self.token).build()
        self.setup_handlers()

    def setup_handlers(self):
        self.app.add_handler(CommandHandler('start', self.start_command))
        self.app.add_handler(CommandHandler('help', self.help_command))

    async def start_command(self, update: Update, context: ContextTypes.DEFAULT_TYPE):
        username = update.effective_user.username
        chat_id = update.message.chat_id
        print(chat_id)
        await update.message.reply_text(f"Hello {username}, now you can receive your messages here")


    async def send_message(self,chat_id,message):
        print(f"Sending message to {chat_id}: {message}")
        await self.app.bot.send_message(text=message,chat_id=chat_id)


    async def help_command(self, update: Update, context: ContextTypes.DEFAULT_TYPE):
        await update.message.reply_text("You need not do anything you just need to start the bot and you are good to go")
        


    def run(self):
        print("Starting bot...")
        self.app.run_polling(poll_interval=3)

if __name__ == "__main__":
    bot = WeatherMonitering()
    bot.run()