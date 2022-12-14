import math
import sys

class Kmeans:
    DEFAULT_ITERATIONS = "200"
    EPSILON = 0.001
    MAX_ITERATIONS = 1000

    def __init__(self, k, input_file_name, iterations):
        self.data_points = self.parse_data_file(input_file_name)
        self.validate_parameters(k, self.data_points, iterations)
        self.iterations = int(iterations)
        self.clusters = self.create_clusters(int(k), self.data_points)
        

    def validate_parameters(self, k, data_points, iterations):
        if not k.isdecimal():
            raise ValueError("Invalid number of clusters!")

        k = int(k)
        if k < 1 or k > len(data_points):
            raise ValueError("Invalid number of clusters!")

        if not iterations.isdecimal():
            raise ValueError("Invalid number of iterations!")

        iterations = int(iterations)
        if iterations < 1 or iterations > self.MAX_ITERATIONS:
            raise ValueError("Invalid number of iterations!")


    def parse_data_file(self, input_file_name):
        data_points = []
        with open(input_file_name, 'r') as file:
            for line in file.readlines():
                data_points.append(self.parse_single_point(line))

        return data_points
    
    @staticmethod
    def parse_single_point(point):
        return [float(num) for num in point.replace("\n", "").split(",")]

    @staticmethod
    def create_clusters(k, data_points):
        clusters = []
        for data_point in data_points[:k]:
            clusters.append(Cluster(data_point))
        return clusters


    def find_k_means(self):
        while self.iterations > 0 and self.centeroids_grater_than_epsilon():
            self.remove_points_from_clusters()
            self.add_points_to_closest_cluster()
            self.update_clusters_centeroids()

            self.iterations -= 1
        
    def add_points_to_closest_cluster(self):
        for data_point in self.data_points:
            self.add_point_to_closest_cluster(data_point)
        
    def add_point_to_closest_cluster(self, data_point):
        closest_cluster = self.clusters[0]
        distance_to_closest_cluster = closest_cluster.calc_distance(data_point)
        for cluster in self.clusters[1:]:
            distance = cluster.calc_distance(data_point)
            if distance < distance_to_closest_cluster:
                closest_cluster = cluster
                distance_to_closest_cluster = distance
    
        closest_cluster.add_point(data_point)

    def update_clusters_centeroids(self):
        for cluster in self.clusters:
            cluster.update_centeroid()

    def remove_points_from_clusters(self):
        for cluster in self.clusters:
            cluster.remove_points()

    def centeroids_grater_than_epsilon(self):
        for cluster in self.clusters:
            if cluster.centeroids_delta_grater_than_epsilon(self.EPSILON):
                return True

        return False

    def __str__(self):
        return "\n".join([str(cluster) for cluster in self.clusters]) + "\n"


class Cluster:
    def __init__(self, centeroid):
        self.current_centeroid = centeroid
        self.previous_centeroid = None
        self.data_points = []

    def add_point(self, point):
        self.data_points.append(point)

    def calc_distance(self, point):
        delta = 0
        for i, j in zip(self.current_centeroid, point):
            delta += pow(i - j, 2)

        return math.sqrt(delta)

    def update_centeroid(self):
        if len(self.data_points) == 0:
            return 
        
        self.previous_centeroid = self.current_centeroid
        
        # Transpose the self.data_points matrix.
        # For example:
        #      From - [[a_1, a_2, a_3], [b_1, b_2, b_3], [c_1, c_2, c_3]] 
        #      To   - [[a_1, b_1, c_1], [a_2, b_2, c_2], [a_3, b_3, c_3]]
        data_points_transpose = list(map(list, zip(*self.data_points)))
        self.current_centeroid = [(1 / len(self.data_points)) * sum(i) for i in data_points_transpose]

    def remove_points(self):
        self.data_points = []

    def centeroids_delta_grater_than_epsilon(self, epsilon):
        if self.previous_centeroid is None:
            return True

        return self.calc_distance(self.previous_centeroid) > epsilon

    def __str__(self):
        return ",".join([f"{round(i, 4):.4f}" for i in self.current_centeroid])


def main():
    k = sys.argv[1]

    if (len(sys.argv) == 4): 
        iterations = sys.argv[2]
        input_file_name = sys.argv[3]
    else:
        iterations = Kmeans.DEFAULT_ITERATIONS
        input_file_name = sys.argv[2]

    try:
        kmeans = Kmeans(k, input_file_name, iterations)
    except ValueError as e:
        print(e)
        return

    kmeans.find_k_means()
    print(kmeans)


if __name__ == '__main__':
    main()
