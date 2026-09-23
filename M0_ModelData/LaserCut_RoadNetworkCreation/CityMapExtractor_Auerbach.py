import osmnx as ox
G = ox.graph.graph_from_place("Auerbach in der Oberpfalz, Bayern, Germany", network_type="drive", simplify=True)
ox.plot.plot_graph(G)