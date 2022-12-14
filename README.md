# DfsOpt
A tool to produce a given number of n most optimal DFS football lineups given
an xml file containing starting requirements, total budget, and player stats 
(position, name, cost, expected points). Simply produces lineups with maximal
point expectation given budget and position constraints. Not the correct tool
if you want to be clever with any particular strategy (stacking or whatever).
Get costs and expected points from wherever you want, none of my business.
Straightforward backtracking-based algorithm to compute optimal lineups.
Usage: "path-to-bin-dir"/dfs-opt -n "num-lineups" -i "path-to-input-file" -o "optional-outfile"
Produces a single lineup if not specified with the "-n" flag. 
Prints to stdout if no outfile. Example inputs are given in the "inputs" directory.
NOTE: As the solution algorithm discovers all possible valid lineups with the given
constraints, run times can get long if too many player options are provided (the larger
sample input runs in ~8 minutes on my machine with an optimized build), so it's
probably advisable to only include players you'd consider starting.

You may optionally specify players that must appear in the final lineups.
Useful if there's a matchup you're higher on than consensus, and additional benefit
of faster run times (smaller search space for valid rosters). See week7-2022-required-players.xml
in sample-inputs for usage example.

This also supports optimizing the Captain Mode, where all players are flex-eligible
and a single player/dst gets 1.5x points for 1.5x cost. see week8-2022-tb-bal-captain.xml
for an example input for this mode.
