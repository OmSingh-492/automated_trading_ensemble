"""
THIS CODE MUST BE RAN AT 3:00 PM DAILY WITH USER FEEDBACK!
"""

import yfinance as yf
import csv
from datetime import datetime, timedelta

def get_parameters():
    parameter1 = 2 # Decides the percentage stop loss.
    parameter2 = 30 # Past 30-day high
    parameter3 = 10 # Past 10-day low
    return parameter1, parameter2, parameter3

# Fetch current high and low for the desired ticker
def get_current_high(ticker):
    today = datetime.now().date()
    hist = yf.Ticker(ticker).history(interval="1m", start=today, end=today + timedelta(days=1))
    if not hist.empty:
        current_price = hist["High"].max()
        return current_price
    else:
        return -1
    
def get_current_low(ticker):
    today = datetime.now().date()
    hist = yf.Ticker(ticker).history(interval="1m", start=today, end=today + timedelta(days=1))
    if not hist.empty:
        current_price = hist["Low"].min()
        return current_price
    else:
        return -1

# Fetch historical data
def past_high(ticker, parameter2):
    data = yf.download(ticker, period="1mo")
    return data["High"].tail(parameter2).max()

def past_low(ticker, parameter3):
    data = yf.download(ticker, period="1mo")
    return data["Low"].tail(parameter3).min()

# To modify the current portfolio.
def modify_holdings(parameter1, parameter2, parameter3, path="./current_holdings.csv"):
    # Checking to exit out of the current bunch of tickers.
    to_remove, to_write = set(), dict()
    with open(path, mode="r") as file:
        csv_reader = csv.reader(file)
        for row in csv_reader:
            ticker = row[0]
            stop_loss = float(row[1])
            current_price = get_current_low(ticker)
            if(current_price == -1):
                continue
            low = past_low(ticker, parameter3)
            if current_price <= stop_loss or current_price <= low:
                to_remove.add(ticker)
              
    # Checking for new entries.
    with open("./tickers.txt", "r") as file:
        for line in file:
            ticker = line.strip()
            if ticker in to_write or ticker in to_remove:
                continue
            current_price = get_current_high(ticker)
            high = past_high(ticker, parameter2)
            if current_price >= high:
                to_write[ticker] = current_price * (1 - parameter1 / 100)
    
    # Modifying the current portfolio.
    with open(path, mode="w", newline="") as file:
        csv_writer = csv.writer(file)
        for ticker, stop_loss in to_write.items():
            csv_writer.writerow([ticker, stop_loss])

if __name__ == "__main__":
    p1, p2, p3 = get_parameters()
    modify_holdings(p1, p2, p3)