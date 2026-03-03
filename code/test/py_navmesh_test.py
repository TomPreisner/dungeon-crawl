import sys
import json
import pytest
from pathfinder.navmesh.navmesh_graph import NavmeshGraph as nmGraph

@pytest.fixture
def generate_grid_graph():
    def load_json(file_name):
        # load the provided test json file and return the graph
        try:
            with open("code\\test\\test_data\\" + file_name, 'r') as json_file:
                data = json.load(json_file)
                graph = nmGraph(data["positions"], data["vertices"], data["edges"])
                return graph
        except FileNotFoundError:
            print(f"The file was not found: {file_name}")
        except json.JSONDecodeError as e:
            print(f"Invalid JSON {file_name}: {e}")
    return load_json

def test_pynavmesh_invalid_start(generate_grid_graph):    
    graph = generate_grid_graph("navmesh_testdata.json")
    start = -1
    end = 54
    path = []
    with pytest.raises(KeyError) as exc_info:
        path = graph.search(start, end)
        
    assert exc_info.value.args[0] == -1
    assert len(path) == 0
    
def test_pynavmesh_invalid_end(generate_grid_graph):    
    graph = generate_grid_graph("navmesh_testdata.json")
    start = 54
    end = graph.get_vertex_count() + 1
    path = []
    with pytest.raises(KeyError) as exc_info:
        path = graph.search(start, end)
        
    assert exc_info.value.args[0] == 101
    assert len(path) == 0

def test_pynavmesh_start_not_connected(generate_grid_graph):    
    graph = generate_grid_graph("navmesh_testdata.json")
    start = 0
    end = 98
    path = graph.search(start, end)

    assert len(path) == 0

def test_pynavmesh_end_not_connected(generate_grid_graph):    
    graph = generate_grid_graph("navmesh_testdata.json")
    start = 10
    end = 99
    path = graph.search(start, end)

    assert len(path) == 0

def test_pynavmesh_adjacent_points(generate_grid_graph):    
    graph = generate_grid_graph("navmesh_testdata.json")
    start = 10
    end = 11
    path = graph.search(start, end)

    assert len(path) == 2

def test_pynavmesh_standard_nav(generate_grid_graph):
    expected_path = []
    try:
        with open("code\\test\\test_data\\navmesh_testdata.json", 'r') as json_file:
            data = json.load(json_file) 
            expected_path = data["standard_path"]
    except FileNotFoundError:
        print(f"The file was not found: navmesh_testdata.json")
    except json.JSONDecodeError as e:
        print(f"Invalid JSON navmesh_testdata.json: {e}")

    assert len(expected_path) > 0

    graph = generate_grid_graph("navmesh_testdata.json")
    start = 89
    end = 79
    path = graph.search(start, end)

    assert path == expected_path
    assert len(path) == len(expected_path)


def test_pynavmesh_long_unconnected_nav(generate_grid_graph):    
    graph = generate_grid_graph("navmesh_testdata.json")
    start = 89
    end = 56
    path = graph.search(start, end)

    assert len(path) == 0

def test_pynavmesh_worst_case():
    
    # This worst case is a large grid where there is only one path from start to end
    #  a snaking path that touches each and every entry in the graph.
    # This is a pressure case to see at what point this library starts to fall break

    # Testing did occur with a 5000x5000 but the test ran so long it wasn't worth it
    # 2500x2500 should be enough to test the viability of the library
    rows = 2500
    cols = 2500

    positions = [(float(i), 0.0, float(j)) for i in range(rows) for j in range(cols)]
    vertices = [cols*i+j for i in range(rows) for j in range(cols)]
    edges = []
    for i in range(rows):
        for j in range(cols):
            if i < rows - 1:
                edges.append(((i*rows)+j, ((i+1)*rows)+j))
            if j < cols -1:
                if (j%2 == 0):
                    if (i == rows-1):
                        edges.append(((i*rows)+j, (i*rows)+j+1))
                else:
                    if (i == 0):
                        edges.append(((i*rows)+j, (i*rows)+j+1))

    graph = nmGraph(positions, vertices, edges)
    start = 0
    end = rows - 1
    path = graph.search(start, end)

    assert len(vertices) == len(path)

if __name__ == "__main__":
    sys.exit(pytest.main([__file__]))
