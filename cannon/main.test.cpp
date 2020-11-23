#define APPROVALS_CATCH
#include <thirdparty/approval_tests/ApprovalTests.hpp>

using namespace ApprovalTests;

auto directoryDisposer = Approvals::useApprovalsSubdirectory("approval_tests");
