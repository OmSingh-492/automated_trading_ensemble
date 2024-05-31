# algorithmic_trading
 Hypothesis: History repeats itself periodically.
 
This project is aimed at automating trades assuming history repeats, and similar patterns can be found out historically
which are existent at the present times. If there is enough evidence from the past suggesting shorting or going long is
a good option, we can do that!
Eventual goal is to combine this approach with some momentum-trading strategy to create an ensemble, and then automating
the trades using an external API.
However, the scope of the current repository is limited to creating the first level of strategy. I would post the merge 
code and the final API integration if I feel so in a separate respository.

For the current repository, any constructive suggestions regarding improvements/better partition of the data points are
welcome!
Languages used: Python, C++


UPDATE-1:
I have extended the current repository, and added the actual trading algorithm too. A suggested portfolio shall be
obtained, on running the file final_strategy.py. It would automatically populate the CSV file, current_holdings.csv.
A list of all tickers which are under consideration in this ensemble are in tickers.txt.
