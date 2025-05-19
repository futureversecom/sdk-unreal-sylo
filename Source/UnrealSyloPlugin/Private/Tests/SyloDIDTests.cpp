#include "SyloDID.h"
#include "Misc/AutomationTest.h"
#include "Containers/UnrealString.h"
#include "Misc/AutomationTest.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FParseSyloDIDTest, "Project.Sylo.ParseDID.BasicParsing", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FParseSyloDIDTest::RunTest(const FString& Parameters)
{
	const FString TestDID = TEXT("did:sylo-data:0xfFFFffFF0000000000000000000000000000052f/da7eb2fc-503b-4998-b2b1-bed89bd49a6e?resolver=fv-sylo-resolver-dev&env=dev");

	FSyloDID Parsed(TestDID);

	TestEqual(TEXT("Method should be 'sylo-data'"), Parsed.SyloDataMethod, TEXT("sylo-data"));
	TestEqual(TEXT("Data owner should match"), Parsed.DataOwner, TEXT("0xfFFFffFF0000000000000000000000000000052f"));
	TestEqual(TEXT("Data ID should match"), Parsed.DataId, TEXT("da7eb2fc-503b-4998-b2b1-bed89bd49a6e"));
	TestEqual(TEXT("Should have 2 query arguments"), Parsed.Args.Num(), 2);
	TestEqual(TEXT("Resolver param should be correct"), Parsed.Args[TEXT("resolver")], TEXT("fv-sylo-resolver-dev"));
	TestEqual(TEXT("Env param should be correct"), Parsed.Args[TEXT("env")], TEXT("dev"));

	return true;
}
