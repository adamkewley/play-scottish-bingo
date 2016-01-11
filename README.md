# Play Scottish Bingo
A console program for simulating a game of card bingo, as described in my [blog post](http://www.adamkewley.com/programming/2015/09/30/lets-play-scottish-bingo.html).

# Usage
`play-scottish-bingo` is a console program that takes the number of players (`number_of_players`) and number of rounds (`number_of_rounds`) as an argument. It writes `number_of_rounds` lines to the standard output. Each line contains the winning round number:

```
$ play-scottish-bingo 2 5
42
48
44
45
45
```
In that case, two people played five rounds. The rounds lasted 42, ..., 45 rounds long.

# Options
By default `play-scottish-bingo` assumes that each player recieves cards from a "fresh" dech (`deck-per-hand`). Alternatively, the dealer can also deal cards from a deck until all cards are exhausted (`exhaust-deck`) using the -s command:

```
$ play-scottish-bingo -s exhaust-deck 7 3
39
41
45
```

# Building
`play-scottish-bingo` was written in ansi C and is compiled using clang (3.6.0). GNU make (3.81) was used for building and linking. The following command builds the project and outputs the resulting binary into a `dist/` directory:

`make all`

