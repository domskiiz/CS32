#include <iostream>
#include <algorithm>
#include <numeric>  // for std::accumulate
#include <utility>
#include <vector>
#include <string>
#include <random>
#include <cassert>

using namespace std;

//========================================================================
//  Set this to false to skip the insertion sort tests; you'd do this if
//  you're sorting so many items that insertion_sort would take more time
//  than you're willing to wait.

const bool TEST_INSERTION_SORT = true;

//========================================================================

//========================================================================
// Timer t;                 // create a timer
// t.start();               // start the timer
// double d = t.elapsed();  // milliseconds since timer was last started
//========================================================================

#include <chrono>

class Timer
{
public:
    Timer()
    {
        start();
    }
    void start()
    {
        m_time = std::chrono::high_resolution_clock::now();
    }
    double elapsed() const
    {
        std::chrono::duration<double,std::milli> diff =
        std::chrono::high_resolution_clock::now() - m_time;
        return diff.count();
    }
private:
    std::chrono::high_resolution_clock::time_point m_time;
};

//========================================================================

// These are used for random number generation

std::random_device aRandomDevice;
std::default_random_engine generator(aRandomDevice());

// Here's a class that is not cheap to copy because the objects contain a
// large array.

// We'll simplify writing our timing tests by declaring everything public
// in this class.  (We wouldn't make data public in a class intended for
// wider use.)

using IdType = int;

struct Sensor
{
    IdType id;
    double avg;
    static const int NREADINGS = 200;
    double readings[NREADINGS];
    Sensor(IdType i) : id(i)
    {
        std::uniform_int_distribution<> distro(0, 99);
        
        // create random sensor readings (from 0 to 99)
        for (size_t k = 0; k < NREADINGS; k++)
            readings[k] = distro(generator);
        // (accumulate computes 0.0 + readings[0] + readings[1] + ...)
        avg = accumulate(readings, readings + NREADINGS, 0.0) / NREADINGS;
    }
};

inline
bool compareSensor(const Sensor& lhs, const Sensor& rhs)
{
    // The Sensor with the higher average should come first.  If they have
    // the same average, then the Sensor with the smaller id number should
    // come first.  Return true iff lhs should come first.  Notice that
    // this means that a false return means EITHER that rhs should come
    // first, or there's a tie, so we don't care which comes first,
    
    if (lhs.avg > rhs.avg)
        return true;
    if (lhs.avg < rhs.avg)
        return false;
    return lhs.id < rhs.id;
}

inline
bool compareSensorPtr(const Sensor* lhs, const Sensor* rhs)
{
    return compareSensor(*lhs, *rhs);
}

bool isSorted(const vector<Sensor>& s)
{
    // Return true iff the vector is sorted according to the ordering
    // relationship compareSensor
    
    for (size_t k = 1; k < s.size(); k++)
    {
        if (compareSensor(s[k],s[k-1]))
            return false;
    }
    return true;
}

void insertion_sort(vector<Sensor>& s, bool comp(const Sensor&, const Sensor&))
{
    // TODO: Implement insertion sort function
    for (int pos = 1; pos < s.size(); pos++)
    {
        Sensor sortMe = s[pos];
        int newPos = pos;
        while ((newPos > 0) && comp(sortMe, s[newPos - 1])) {
            s[newPos] = s[newPos - 1];
            newPos--;
        }
        s[newPos] = sortMe;
    }
}

// Report the results of a timing test

void report(string caption, double t, const vector<Sensor>& s)
{
    cout << t << " milliseconds; " << caption
    << "; first few sensors are\n\t";
    size_t n = s.size();
    if (n > 5)
        n = 5;
    for (size_t k = 0; k < n; k++)
        cout << " (" << s[k].id << ", " << s[k].avg << ")";
    cout << endl;
}

int main()
{
    size_t nsensors;
    cout << "Enter number of sensors to sort: ";
    cin >> nsensors;
    if ( !cin  ||  nsensors <= 0)
    {
        cout << "You must enter a positive number.  Goodbye." << endl;
        return 1;
    }
    
    // Create a random ordering of id numbers 0 through nsensors-1
    vector<IdType> ids;
    for (size_t j = 0; j < nsensors; j++)
        ids.push_back(IdType(j));
    shuffle(ids.begin(), ids.end(), generator);  // from <algorithm>
    
    // Create a bunch of Sensors
    vector<Sensor> unorderedSensors;
    for (size_t k = 0; k < ids.size(); k++)
        unorderedSensors.push_back(Sensor(ids[k]));
    
    // Create a timer
    
    Timer timer;
    
    // Sort the Sensors using the STL sort algorithm.  It uses a variant
    // of quicksort called introsort.
    
    vector<Sensor> sensors(unorderedSensors);
    timer.start();
    sort(sensors.begin(), sensors.end(), compareSensor);
    report("STL sort", timer.elapsed(), sensors);
    assert(isSorted(sensors));
    
    // Sort the already sorted array using the STL sort.  This should be
    // fast.
    
    timer.start();
    sort(sensors.begin(), sensors.end(), compareSensor);
    report("STL sort if already sorted", timer.elapsed(), sensors);
    assert(isSorted(sensors));
    
    if (TEST_INSERTION_SORT)
    {
        // Sort the original unsorted array using insertion sort.  This
        // should be really slow.  If you have to wait more than a minute,
        // try rerunning the program with a smaller number of Sensors.
        
        sensors = unorderedSensors;
        timer.start();
        insertion_sort(sensors, compareSensor);
        report("insertion sort if not already sorted", timer.elapsed(), sensors);
        assert(isSorted(sensors));
        
        // Sort the already sorted array using insertion sort.  This should
        // be fast.
        
        timer.start();
        insertion_sort(sensors, compareSensor);
        report("insertion sort if already sorted", timer.elapsed(), sensors);
        assert(isSorted(sensors));
    }
    
    // Since Sensors are expensive to copy, and since the STL's sort copies
    // Sensors O(N log N) times, let's sort POINTERS to the Sensors, then
    // make one final pass to rearrange the Sensors according to the
    // reordered pointers.  We'll write some code; you write the rest.
    
    // Set sensors to the original unsorted sequence
    sensors = unorderedSensors;
    
    // Start the timing
    timer.start();
    
    // Create an auxiliary copy of sensors to faciliate the later reordering.
    // We create it in a local scope so that we also account for the
    // destruction time.
    {
        vector<Sensor> auxSensors(sensors);
        
        // TODO:  Create a vector of Sensor pointers, and set each pointer
        //        to point to the corresponding Sensor in auxSensors.
        vector<Sensor*> vec;
        
        vector<Sensor>::iterator it;
        it = auxSensors.begin();
        
        while (it != auxSensors.end()) {
            vec.push_back(&(*it));
            it++;
        }
        // TODO:  Sort the vector of pointers using the STL sort algorithm
        //        with compareSensorPtr as the ordering relationship.
        sort(vec.begin(), vec.end(), compareSensorPtr);
        
        // TODO:  Using the now-sorted vector of pointers, replace each Sensor
        //        in sensors with the Sensors from auxSensors in the correct
        //        order.
        for (int i = 0; i < vec.size(); i++) {
            sensors[i] = *vec[i];
        }
        
    } // auxSensors will be destroyed here
    
    // Report the timing and verify that the sort worked
    report("STL sort of pointers", timer.elapsed(), sensors);
    assert(isSorted(sensors));
}
