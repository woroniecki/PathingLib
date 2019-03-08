<h1>Pathing DLL</h1>

Allows creating graphs and finding paths by using Dijkstra or ALT algorithms.

<h2>Example Usage</h2>

<h3>Dijkstra</h3>

```cpp
PathingLib::Graph g = PathingLib::Graph::loadGraphFromFile("graphNodes.txt", "graphEdges.txt");
PathingLib::Path p = PathingLib::Dijkstra::getPath(0, 5, g);
cout << "Path distance: " << p.getDistance('m');
```

<h3>ALT</h3>

```cpp
PathingLib::Graph g = PathingLib::Graph::loadGraphFromFile("graphNodes.txt", "graphEdges.txt");
PathingLib::ALT alt(g, 4);
PathingLib::Path p = alt.getPath(0, 1000);
cout << "Path distance: " << p.getDistance('m');
```

<div>
For better clarity see 
<a href="https://woroniecki.github.io/PathingLib/index.html">documentation</a> and 
<a href="https://github.com/woroniecki/PathingLib/tree/master/PathingLib/ExampleClient">example client</a>.
</div>

<h3>Graphic result</h3>
Result of example client.
<img src="https://lh3.googleusercontent.com/lrrYkXnAi0vnaDzvVQaOz676VlDZkA85nscsgpleu0cgi1DkTjjJFeq8BvYwsLx4Ec_pKgeyFaAiFaOoMei65JgE2e51kgE1DYx4gp5Y3IdFCps-UMqpwbo1uFDJeqZu6OGv=w678-h451-no">

<h2>Made by using</h2>
<ul>
<li>C++</li>
<li>Microsoft Visual Studio 2017</li>
<li>Microsoft Visual C++ 2017 14.10.25008</li>
</ul>

<h2>License</h2>
<a href="https://github.com/woroniecki/PathingLib/blob/master/LICENSE">MIT</a>
