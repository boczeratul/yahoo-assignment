#include <iostream>
#include <vector>
#include <queue>

#define TIME_TYPE int

namespace VisitorTracking {
	enum VisitEventType { ENTER, LEAVE };

	class Visitor {
	public:
		int ID;
		TIME_TYPE enterTime, leaveTime;
	};

	class VisitEvent {
	public:
		TIME_TYPE time;
		int ID;
		VisitEventType type;
	};
}

using namespace std;
using namespace VisitorTracking;

struct comparator{
	bool operator() (VisitEvent a, VisitEvent b) {
		return a.time > b.time;
	}
};

typedef priority_queue<VisitEvent, vector<VisitEvent>, comparator> MinHeap;

int main(int argc, char *argv[]) {
	
	int visitorCount;
	cout << "Number of visitors: ";
	cin >> visitorCount;

	Visitor *visitors = new Visitor[visitorCount];
	for (int i = 0; i < visitorCount; i++) {
		cout << "ID enterTime leaveTime: ";
		cin >> visitors[i].ID >> visitors[i].enterTime >> visitors[i].leaveTime;
	}

	TIME_TYPE startTime, endTime;
	cout << "Time of interest(startTime endTime):";
	cin >> startTime >> endTime;
	
	// first find the visitors that showed up at the interval of interest
	int currentCount = 0;
	vector<int> guestList;
	MinHeap eventList;
	for (int i = 0; i < visitorCount; i++) {
		if (visitors[i].enterTime < endTime && visitors[i].leaveTime > startTime) {
			if (visitors[i].enterTime > startTime) 
			{
				VisitEvent event;
				event.time = visitors[i].enterTime;
				event.type = ENTER;
				event.ID = visitors[i].ID;
				eventList.push(event);
			}
			else {
				currentCount++;
				guestList.push_back(visitors[i].ID);
			}

			if (visitors[i].leaveTime < endTime) 
			{
				VisitEvent event;
				event.time = visitors[i].leaveTime;
				event.type = LEAVE;
				event.ID = visitors[i].ID;
				eventList.push(event);
			}
		}
	}

	int maxCount = currentCount;
	vector<TIME_TYPE> maxStartTime;
	vector<TIME_TYPE> maxEndTime;
	vector< vector<int> > maxGuestList;
	maxStartTime.push_back(startTime);
	while ( !eventList.empty() ) {
		VisitEvent event = eventList.top();
		if ( event.type == ENTER ) {
			currentCount++;
			guestList.push_back(event.ID);
			if (currentCount > maxCount) {
				maxStartTime.clear();
				maxEndTime.clear();
				maxStartTime.push_back(event.time);

				maxGuestList.clear();
				maxGuestList.push_back(guestList);
				maxCount = currentCount;
			}
			else if (currentCount == maxCount)
			{
				maxStartTime.push_back(event.time);
				maxGuestList.push_back(guestList);
			}
			cout << event.ID << " entered at time: " << event.time << endl;
		}
		else {
			if (currentCount == maxCount)
				maxEndTime.push_back(event.time);
			currentCount--;
			guestList.erase(std::remove(guestList.begin(), guestList.end(), event.ID), guestList.end());
			cout << event.ID << " left at time: " << event.time << endl;
		}
		eventList.pop();
    }
    if (currentCount == maxCount)
		maxEndTime.push_back(endTime);

    cout << "Max count: " << maxCount << endl;
    for (int i = 0; i < maxStartTime.size(); i++ ) {
    	cout << "At time interval: " << maxStartTime[i] << " to " << maxEndTime[i] << endl;
    	cout << "\tVisitors: ";
    	for (int j = 0; j < maxGuestList[i].size(); j++ ) 
    		cout << maxGuestList[i][j] << " ";
    	cout << endl;
    }

	delete[] visitors;

	return 0;
}