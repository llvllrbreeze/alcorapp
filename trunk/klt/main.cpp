#include "alcor/core/klt_tracker_t.h"
#pragma comment (lib,"libklt.lib")

using namespace all;

int main()
{
  all::core::klt_tracker_t klt;
  klt.init();

  getchar();
}