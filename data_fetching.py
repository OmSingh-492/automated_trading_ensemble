import yfinance as yf
import pandas as pd
from datetime import datetime

def file_creation(ticker, start_date="1990-01-01", end_date="2020-05-01"):
    data = yf.download(ticker, start=start_date, end=end_date)
    data["Change"] = data["Close"].pct_change().dropna() * 100
    with open(f"./outputs/{ticker}.txt", "w") as file:
        for change in data["Change"]:
            file.write(f"{change}\n")
    data = yf.download(ticker, start=end_date, end=datetime.today().strftime('%Y-%m-%d'))
    data["Change"] = data["Close"].pct_change().dropna() * 100
    with open(f"./backtest_files/{ticker}.txt", "w") as file:
        for change in data["Change"]:
            file.write(f"{change}\n")

if __name__ == "__main__":
    with open("./tickers.txt", "r") as file:
        for line in file:
            file_creation(line.strip())