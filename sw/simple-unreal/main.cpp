struct UnrealData {
    char Level;
    char Component;
    char Property;
    char Value;

    UnrealData() {

    };
};

UnrealData* volatile const unrealData = (UnrealData * volatile const)(0x73000000);

using namespace std;

int main() {

	//unrealData->Level = 'g';

	//printf("Test");

	return 0;
}
