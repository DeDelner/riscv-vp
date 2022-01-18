struct UnrealData {
/*     enum class Command : uint8_t {
		NONE = 0,
		CALL,
		PROPERTY
	} volatile command; */

    char objectPath;

    UnrealData() {

    };
};

UnrealData* volatile const unrealData = (UnrealData * volatile const)(0x73000000);

using namespace std;

int main() {

	unrealData->objectPath = 'g';

	//printf("Test");

	return 0;
}