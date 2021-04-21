#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <iterator>
#include <iostream>
#include <functional>

using namespace std;

using namespace sf;

int screenWidth = 1100; //bars divided by pixels so propper division is necessary
int screenHight = 1000;

int swaps = 0;

void render(RenderWindow& window, VertexArray *bars)
{
	window.clear();
	window.draw(*bars);
	window.display();
}

vector <Vertex*> colouredBars;
void fadeColour(vector <Vertex*> *colouredBars)
{
	for (auto vertex = (*colouredBars).begin(); vertex != (*colouredBars).end(); )
	{
		if ((**vertex).color.r != 255)
		{
			(**vertex).color.r += 1;
			++vertex;
		}
		else
		{
			vertex = (*colouredBars).erase(vertex);
		}
	}
}

void disappearingBars(VertexArray *bars, int *NBars)
{
	(*bars)[(*NBars * 4) - 4].position = sf::Vector2f(0, 0);//x1, y2
	(*bars)[(*NBars * 4) - 3].position = sf::Vector2f(0, 0);//x2, y2
	(*bars)[(*NBars * 4) - 2].position = sf::Vector2f(0, 0);//x2, y1
	(*bars)[(*NBars * 4) - 1].position = sf::Vector2f(0, 0);//x1, y1	
	*NBars -= 1;
}

void colourfulBars(VertexArray *bars, int *NBars)
{
	(*bars)[(*NBars * 4) - 4].color.r = 0;
	(*bars)[(*NBars * 4) - 3].color.r = 0;
	(*bars)[(*NBars * 4) - 2].color.r = 0;
	(*bars)[(*NBars * 4) - 1].color.r = 0;

	colouredBars.push_back(&(*bars)[(*NBars * 4) - 4]);
	colouredBars.push_back(&(*bars)[(*NBars * 4) - 3]);
	colouredBars.push_back(&(*bars)[(*NBars * 4) - 2]);
	colouredBars.push_back(&(*bars)[(*NBars * 4) - 1]);

	*NBars -= 1;
}


void bubbleSort(VertexArray *bars, int *NBars, RenderWindow& window)
{
	bool swap = true;
	while (swap == true)
	{
		swap = false;
		int barsChecked = 0;//may need fiddleing
		while (barsChecked < *NBars - 1)
		{
			//if bar1 > bar2
			if ((*bars)[(barsChecked * 4) + 0].position.y > (*bars)[(barsChecked * 4) + 4].position.y)
			{
				int firstBarY = (*bars)[(barsChecked * 4) + 0].position.y;
				//alter Y of bar1
				(*bars)[(barsChecked * 4) + 0].position.y = (*bars)[(barsChecked * 4) + 4].position.y;
				(*bars)[(barsChecked * 4) + 1].position.y = (*bars)[(barsChecked * 4) + 4].position.y;
				//alter Y of bar2
				(*bars)[(barsChecked * 4) + 4].position.y = firstBarY;
				(*bars)[(barsChecked * 4) + 5].position.y = firstBarY;
				swaps++;
				swap = true;
			}
			barsChecked += 1;
			//render(window, bars);
		}
		render(window, bars);
	}
}

int partition(VertexArray *bars, int low, int high, RenderWindow& window)
{
	int pivot = (*bars)[high * 4].position.y; // pivot  
	int i = (low - 1); // Index of smaller element  

	for (int j = low; j <= high - 1; j++)
	{
		// If current element is smaller than the pivot  
		if ((*bars)[j * 4].position.y < pivot)
		{
			i++; // increment index of smaller element  
			int firstBarY = (*bars)[(i * 4) + 0].position.y;
			//alter Y of bar1
			(*bars)[(i * 4) + 0].position.y = (*bars)[(j * 4) + 0].position.y;
			(*bars)[(i * 4) + 1].position.y = (*bars)[(j * 4) + 1].position.y;
			//alter Y of bar2
			(*bars)[(j * 4) + 0].position.y = firstBarY;
			(*bars)[(j * 4) + 1].position.y = firstBarY;
			swaps++;
			render(window, bars);
		}
	}
	//swap(&arr[i + 1], &arr[high]);
	int firstBarY = (*bars)[(i + 1) * 4].position.y;// MABY I SHOULD HAVE 4 ADDED IN THIS SECTION?
													//alter Y of bar1
	(*bars)[(i + 1) * 4].position.y = (*bars)[high * 4].position.y;
	(*bars)[((i + 1) * 4) + 1].position.y = (*bars)[high * 4].position.y;
	//alter Y of bar2
	(*bars)[high * 4].position.y = firstBarY;
	(*bars)[(high * 4) + 1].position.y = firstBarY;
	swaps++;
	render(window, bars);
	return (i + 1);
}

/* The main function that implements QuickSort
arr[] --> Array to be sorted,
low --> Starting index,
high --> Ending index */
void quickSort(VertexArray *bars, int low, int high, RenderWindow& window)
{
	if (low < high)
	{
		//render(window, bars);
		/* pi is partitioning index, arr[p] is now
		at right place */
		int partitionIndex = partition(bars, low, high, window);

		// Separately sort elements before  
		// partition and after partition  
		quickSort(bars, low, (partitionIndex - 1), window);
		quickSort(bars, (partitionIndex + 1), high, window);
	}
}


void populateHightVector(vector<int> *hights)
{
	// First create an instance of an engine.
	random_device rnd_device;
	// Specify the engine and distribution.
	mt19937 mersenne_engine{ rnd_device() };  // Generates random integers
	uniform_int_distribution<int> dist{ 50, (screenHight - 100) };//50 for padding

	auto gen = [&dist, &mersenne_engine]() {
		return dist(mersenne_engine);
	};
	int n = 1;
	generate(begin(*hights), end(*hights), gen);
}

void populateBarsVector(VertexArray *bars, int *NBars, vector<int> *hights)
{
	float barWidth = ((screenWidth - 100) / *NBars);
	//cout << barWidth << endl;
	int barsCreated = 0;
	while (barsCreated < *NBars)
	{
	//	x1 = 50 + (barsCreated * width), x2 = 50 + (BarsCreated * width) + width
		//y1 = screenHight, y2 = screenHight - height
		(*bars)[(barsCreated * 4) + 0].position = sf::Vector2f((50 + (barsCreated * barWidth)), screenHight - (*hights)[0]);//x1, y2
		(*bars)[(barsCreated * 4) + 1].position = sf::Vector2f(50 + (barsCreated * barWidth) + barWidth, screenHight - (*hights)[0]);//x2, y2
		(*bars)[(barsCreated * 4) + 2].position = sf::Vector2f(50 + (barsCreated * barWidth) + barWidth, screenHight - 50);//x2, y1
		(*bars)[(barsCreated * 4) + 3].position = sf::Vector2f((50 + (barsCreated * barWidth)), screenHight - 50);//x1, y1
		
		(*hights).erase((*hights).begin());
		barsCreated += 1;
	}
	//cout << barsCreated;
}



int main()
{
	RenderWindow window(VideoMode(screenWidth, screenHight), "SFML works!");
	
	int NBars = 1000;

	vector <int> hights(NBars);
	populateHightVector(&hights);

	VertexArray bars(Quads, 4);
	bars.resize(NBars * 4);
	populateBarsVector(&bars, &NBars, &hights);
	cout << "Enter '1' for bubblesort\nEnter '2' for quicksort" << endl;
	string sortType;
	cin >> sortType;
	if (sortType == "1")
	{
		bubbleSort(&bars, &NBars, window);
	}
	else if (sortType == "2")
	{
		quickSort(&bars, 0, (NBars - 1), window);
	}
	
	cout << "SORTED!!!" << endl;
	cout << "With " << swaps << " swaps";
	//The speed here is deterined by how often render() is called not by the algorithm
	while (window.isOpen())
	{
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
				window.close();
		}
		
		//quickSort(&bars, 0, ((NBars - 1) * 4), window);// THIS IS CALLED MULTIPLE TIMES SO ONLY APPEARS TO WORK
		if (NBars > 0)
		{
			colourfulBars(&bars, &NBars);
		}
		if (colouredBars.size() > 0)
		{
			fadeColour(&colouredBars);
		}
		render(window, &bars);
	}

	return 0;
}