# Formula 1 Standings Paralell Universes

Code to generate the statistics conceptualized by the youtuber [Chain Bear](https://www.youtube.com/c/chainbearf1) in the video [2019 Remixed! - The 27399 ways Max Verstappen could win the 2019 season](https://www.youtube.com/watch?v=jfa5O8sg8g0) but for any season.

The idea here is that in a Formula 1 season there's only 1 driver who will win the Driver's Championship, which will be the one with most points by the end of the season. But what if we could remove some of the races from the seasons? Then the winner of the championship might change since these removed races could be races that favoured one driver than another.

Here we have an experiment which tries to calculate the number of "parallel universes" (theoretical universes where some of the races are missing) that would make each driver on the grid champion. Not only that but which is the longest theoretical season (the season with most races present in it) that would make each driver champion.

For example, the results shown in the video linked (using data from the 2019 Formula 1 season) we had:

1. **Lewis Hamilton** (actual winner) would have won in 2041043 parallel universes with the longest one being 21 races long (the actual season)
2. **Velteri Bottas** would have won in 29567 parallel universes with the longest one being 13 races long
3. **Max Verstappen** would have won in 27399 parallel universes with the longest one being 11 races long
4. **Sebastian Vettel** would have won in 1295 parallel universes with the longest one being 9 races long
5. **Charles Leclerc** would have won in 1031 parallel universes with the longest one being 8 races long
...

<img src="https://i.imgur.com/zpjyLOK.png" alt="chain bear video" width="75%"/>

Frame from [Chain Bear's video](https://www.youtube.com/watch?v=jfa5O8sg8g0)

## How to run it

```g++ main.cpp -o main```

```./main standings/points_table_2020.txt```
