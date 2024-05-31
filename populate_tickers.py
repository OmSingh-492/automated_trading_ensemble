import yfinance as yf

def get_tickers(path="./kaggle_dataset.csv"):
    tickers = []
    with open(path, "r") as file:
        for line in file:
            actual = line.strip()
            ticker = ""
            for ch in actual:
                if ch not in [",", '"', "'"]:
                    ticker += ch
            tickers.append(ticker)
    return tickers
        
def populate_file(path="./tickers.txt"):
    tickers = get_tickers()
    with open(path, "w") as file:
        for ticker in tickers:
            file.write(ticker + "\n")

if __name__ == "__main__":
    populate_file()