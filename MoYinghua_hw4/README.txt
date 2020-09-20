Yinghua Mo hw4

The data txt files are regarded defaultly as saving in the same directory.

p2. The program will finish running on the smaller graph in 1 seconds, finish running on the bigger graph in 35~55 minutes.
result:
graph1 dijkstra_all_src() runtime: 2.037183
graph2 dijkstra_all_src() runtime: 3631.759077

p3. The time complexity should be O(n^2logE / p); suppose E is the number of edges. Because I did not update the distance value of node in heap. I push the new value into the heap, which should be number of edges at most;

result: 
#threads : 4 
graph1 dijkstra_all_src() runtime: 0.649389
graph2 dijkstra_all_src() runtime: 817.674158
Comments: as we can see from the results of p2 and p3, the timing of p3 with parallel decreases linearly comparing to the linear method.

p4.
result:
markov-graph graph_period() runtime: 0.000027
markov-graph graph_period() result:1

p7. potential problem when use the universal hash method. Locally, I can import the large dictionary within a second, but it runs for couples of minutes when I run it on a linux machine.

p8. By default, the code will generate 100 data points for each data stucture originally to test; in order to generate more duplicate data, I would like to reduce the possible kinds of value that can be produced.