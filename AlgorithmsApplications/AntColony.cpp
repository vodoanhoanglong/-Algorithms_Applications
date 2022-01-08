#include <cstdlib> // De tao so random
#include <iostream>
#include <string>
#include <sstream> 
#include <math.h> 
#include <ctime>  // De su dung ham time tao mot so ngau nhien
using namespace std;


#define INVALID -1
#define CITY_AMOUNT 14 // so luong thanh pho
#define POPULATION_SIZE 5 // so luong kien
#define PHEROMONE_RATE 0.1 // nong do mui
#define ALFA 1
#define BETA 2
#define MAX_ITERATIONS 3 // so lan dan kien co the di lai
#define EVAPORATION_RATE 0.5
#define POSITIVE_CONTS 0.75

struct ant {
	double distance;
	double fitness;
	int position;
	int route[CITY_AMOUNT];
};


ant ants[POPULATION_SIZE];
double pheromone_links[CITY_AMOUNT][CITY_AMOUNT];

int distance_links[CITY_AMOUNT][CITY_AMOUNT] = { { INVALID, 11, 20, 27, 40, 43,
		39, 28, 18, 10, 18, 30, 30, 32 }, { 11, INVALID, 9, 16, 29, 32, 28, 19,
		11, 4, 17, 23, 21, 24 }, { 20, 9, INVALID, 7, 20, 22, 19, 15, 10, 11,
		21, 21, 13, 18 }, { 27, 16, 7, INVALID, 13, 16, 12, 13, 13, 18, 26, 21,
		11, 17 },
		{ 40, 29, 20, 13, INVALID, 3, 2, 21, 25, 31, 38, 27, 16, 20 }, { 43,
				32, 22, 16, 3, INVALID, 4, 23, 28, 33, 41, 30, 17, 20 }, { 39,
				28, 19, 12, 2, 4, INVALID, 22, 25, 29, 38, 28, 13, 17 }, { 28,
				19, 15, 13, 21, 23, 22, INVALID, 9, 22, 18, 7, 25, 30 }, { 18,
				11, 10, 13, 25, 28, 25, 9, INVALID, 13, 12, 12, 23, 28 }, { 10,
				4, 11, 18, 31, 33, 29, 22, 13, INVALID, 20, 27, 20, 23 }, { 18,
				17, 21, 26, 38, 41, 38, 18, 12, 20, INVALID, 15, 35, 39 }, {
				30, 23, 21, 21, 27, 30, 28, 7, 12, 27, 15, INVALID, 31, 37 }, {
				30, 21, 13, 11, 16, 17, 13, 25, 23, 20, 35, 31, INVALID, 5 }, {
				32, 24, 18, 17, 20, 20, 17, 30, 28, 23, 39, 37, 5, INVALID } };
				
				
int best_distance = 1000000000;
int worse_distance = 0;
double best_fitness;
double worse_fitness;
int best_route[CITY_AMOUNT];
int worse_route[CITY_AMOUNT];
double average = 0.0;
double variance = 0.0;
double standard_deviation = 0.0;
int greater_distance = INVALID;

void init_ant(int index);
void seed_initial_pheromone(bool random);
void get_greater_distance();
bool check_visit(int ant_index, int position);
void build_solution();
void check_best_distance();
void calculate_fitness();
void pheromone_evaporates();
void update_pheromone();

double calculate_time(clock_t start, clock_t end);
int get_random_number(int from, int to);
void print_route(int ant_index, int route[CITY_AMOUNT], double distance);
void print_pheromone();
string number_to_String(double n);
void print_result();
void calculate_metrics();

int main(int argc, char *argv[]) {
	clock_t time_start = clock();
	// Khoi tao ngau nhien
	srand(time(0));
	// Khoi tao bien dem
	int iteration = 0;

	// �at nong do pheromone ban dau
	seed_initial_pheromone(false);
	// Tinh khoang cach xa nhat
	get_greater_distance();

	// Dieu kien dung
	while (iteration < MAX_ITERATIONS) {
		build_solution();
		check_best_distance();
		calculate_fitness();
		pheromone_evaporates();
		update_pheromone();
		for (int i = 0; i < POPULATION_SIZE; i++) {
			print_route(i, ants[i].route, ants[i].distance);
		}
		iteration++;
	}

	// Xuat ket qua
	print_result();

	cout << "\nThoi gian chay (ACO): "
			<< calculate_time(time_start, clock()) << " ms" << endl;

	return 0;
}

void init_ant(int index) {
	ants[index].distance = 0;
	ants[index].fitness = 0;
	ants[index].position = 0;
	for (int j = 0; j < CITY_AMOUNT; j++) {
		ants[index].route[j] = INVALID;
	}
	// Dat con kien vao 1 thanh pho ngau nhien
	int random_city = get_random_number(0, (CITY_AMOUNT - 1));
	ants[index].route[0] = random_city;
}

void seed_initial_pheromone(bool random) {
	for (int i = 0; i < CITY_AMOUNT; i++) {
		for (int j = 0; j < CITY_AMOUNT; j++) {
			if (i != j) {
				double random_pheromone = (double) get_random_number(0, 100)
						/ 100.0;
				pheromone_links[i][j] = (random == true) ? random_pheromone
						: PHEROMONE_RATE;
			} else {
				pheromone_links[i][j] = INVALID;
			}
		}
	}
}

void get_greater_distance() {
	for (int i = 0; i < CITY_AMOUNT; i++) {
		for (int j = 0; j < CITY_AMOUNT; j++) {
			if (distance_links[i][j] > greater_distance) {
				greater_distance = distance_links[i][j];
			}
		}
	}
	greater_distance += 1;
}

bool check_visit(int ant_index, int position) {
	for (int i = 0; i < CITY_AMOUNT; i++) {
		if (ants[ant_index].route[i] == position) {
			return true;
		}
	}
	return false;
}

void build_solution() {
	// Xay dung dan kien
	for (int i = 0; i < POPULATION_SIZE; i++) {
		// Khoi tao con kien
		init_ant(i);

		// Lap den khi con kien di qua tat ca cac thanh pho
		while (ants[i].position < (CITY_AMOUNT - 1)) {
			int position = ants[i].position;

			double transition_probability[CITY_AMOUNT];

			double link_rate_sum = 0;
			// Them pheromone va ty le heuristic
			for (int j = 0; j < CITY_AMOUNT; j++) {
				// Neu thanh pho da duoc den tham thi khong di vao
				if (check_visit(i, j) == false) {
					if (pheromone_links[position][j] >= 0
							and distance_links[position][j] >= 0) {
						link_rate_sum
								+= pow(pheromone_links[position][j], ALFA)
										* pow(
												greater_distance
														- distance_links[position][j],
												BETA);
					}
				}
			}
			// Tinh xac suat chuyen tiep
			for (int j = 0; j < CITY_AMOUNT; j++) {
				// Neu thanh pho da duoc den tham thi khong di vao
				if (check_visit(i, j) == false) {
					if (pheromone_links[position][j] >= 0
							and distance_links[position][j] >= 0) {
						transition_probability[j] = (pow(
								pheromone_links[position][j], ALFA) * pow(
								greater_distance - distance_links[position][j],
								BETA)) / link_rate_sum;
					} else {
						transition_probability[j] = 0;
					}
				} else {
					transition_probability[j] = 0;
				}
			}

			
			double roulette = (double) get_random_number(0, 100) / 100.0;
			double minor = 0;
			double major = 0;

			// Chon nut tiep theo
			for (int j = 0; j < CITY_AMOUNT; j++) {
				// Neu thanh pho da duoc den tham thi khong di vao
				if (check_visit(i, j) == false) {
					major += transition_probability[j];
					if (roulette >= minor and roulette <= major) {
						ants[i].route[position + 1] = j;
						ants[i].distance += distance_links[position][j];
						ants[i].position += 1;
						break;
					} else {
						minor = major;
					}
				}
			}
		}
	}
}

void check_best_distance() {
	for (int i = 0; i < POPULATION_SIZE; i++) {
		if (ants[i].distance < best_distance) {
			best_distance = ants[i].distance;
			for (int j = 0; j < CITY_AMOUNT; j++) {
				best_route[j] = ants[i].route[j];
			}
		} else if (ants[i].distance > worse_distance) {
			worse_distance = ants[i].distance;
			for (int j = 0; j < CITY_AMOUNT; j++) {
				worse_route[j] = ants[i].route[j];
			}
		}
	}
}

void calculate_fitness() {
	for (int i = 0; i < POPULATION_SIZE; i++) {
		double fitness = (double) ants[i].distance / (double) best_distance;
		if (fitness < best_fitness) {
			best_fitness = fitness;
		} else if (fitness > worse_fitness) {
			worse_fitness = fitness;
		}
		ants[i].fitness = fitness;
	}
}

void pheromone_evaporates() {
	for (int i = 0; i < CITY_AMOUNT; i++) {
		for (int j = 0; j < CITY_AMOUNT; j++) {
			if (pheromone_links[i][j] != INVALID) {
				pheromone_links[i][j] = (1 - EVAPORATION_RATE)
						* pheromone_links[i][j];
			}
		}
	}
}

void update_pheromone() {
	for (int i = 0; i < POPULATION_SIZE; i++) {
		double pheromone_to_sum = POSITIVE_CONTS / ants[i].fitness;
		for (int j = 0; j < (CITY_AMOUNT - 1); j++) {
			int city = ants[i].route[j];
			int next_city = ants[i].route[j + 1];
			if (pheromone_links[city][next_city] != INVALID) {
				pheromone_links[city][next_city] += pheromone_to_sum;
			}
		}
	}
}


int get_random_number(int from, int to) {
	return (from < to) ? (rand() % to) + from : 0;
}

double calculate_time(clock_t start, clock_t end) {
	return 1000.0 * ((double) (end - start) / (double) CLOCKS_PER_SEC);
}

void print_route(int ant_index, int route[CITY_AMOUNT], double distance) {
	string temp = "Vi tri kien " + number_to_String(ant_index) + " : ";
	for (unsigned int i = 0; i < CITY_AMOUNT; i++) {
		temp += number_to_String(route[i]);
		if ((i + 1) != CITY_AMOUNT) {
			temp += ", ";
		}
	}
	temp += ". Khoang cach: " + number_to_String(distance) + "\n";
	cout << temp << endl;
}

void print_pheromone() {
	string temp = "Ty le pheromone: ";
	temp += "\n{";
	for (int i = 0; i < CITY_AMOUNT; i++) {
		temp += "{";
		for (int j = 0; j < CITY_AMOUNT; j++) {
			temp += number_to_String(pheromone_links[i][j]);
			if ((j + 1) != CITY_AMOUNT) {
				temp += ", ";
			}
		}
		if ((i + 1) != CITY_AMOUNT) {
			temp += "}, ";
		} else {
			temp += "}";
		}
	}
	temp += "}\n";
	cout << temp << endl;
}

void print_result() {
	cout << "Ket qua te nhat:" << endl;
	cout << "f(x):" << worse_fitness << endl;
	print_route(0, worse_route, worse_distance);

	cout << "Ket qua tot nhat:" << endl;
	cout << "f(x):" << best_fitness << endl;
	print_route(0, best_route, best_distance);

	calculate_metrics();
	cout << "Trung binh:" << average << endl;
	cout << "Phuong sai:" << variance << endl;
	cout << "Do lech chuan:" << standard_deviation << endl;
}

string number_to_String(double n) {
	stringstream out;
	out << n;
	return out.str();
}

void calculate_metrics() {
	// Tinh trung binh
	double sum = 0;
	for (int i = 0; i < POPULATION_SIZE; i++) {
		sum += ants[i].distance;
	}
	average = (double) sum / (double) POPULATION_SIZE;
	// Tinh phuong sai
	sum = 0;
	for (int i = 0; i < POPULATION_SIZE; i++) {
		sum += pow(ants[i].distance - average, 2);
	}
	variance = (double) sum / (double) POPULATION_SIZE;
	// Tinh do lech chuan
	standard_deviation = pow(variance, 0.5);
}
