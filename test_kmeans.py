import pytest
import os
from kmeans import Kmeans

RESOURCES_PATH = 'resources'

@pytest.mark.parametrize("k, iterations, input_file_name, output_file_name", 
[
    ("3", "600", "input_1.txt", "output_1.txt"), 
    ("7.0", "200", "input_2.txt", "output_2.txt"),
    ("15", "300.0", "input_3.txt", "output_3.txt"),
])
def test_kmeans(k, iterations, input_file_name, output_file_name):
    kmeans = Kmeans(k, os.path.join(RESOURCES_PATH, input_file_name), iterations)
    kmeans.find_k_means()

    with open(os.path.join(RESOURCES_PATH, output_file_name), 'r') as f:
        # since print adds new line
        assert f.read() == str(kmeans) + "\n"
