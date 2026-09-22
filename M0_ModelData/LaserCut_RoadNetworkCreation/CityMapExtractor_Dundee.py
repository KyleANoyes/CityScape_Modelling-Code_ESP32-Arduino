import osmnx as ox
G = ox.graph.graph_from_place("Omaha, Nebraska, USA", network_type="drive", simplify=True)
ox.plot.plot_graph(G)
