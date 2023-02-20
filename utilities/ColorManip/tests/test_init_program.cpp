#include <ColorDiff_OpenCL.h>
#include <iostream>
#include <random>

using std::cout, std::endl;

std::random_device rd;
std::mt19937 mt(rd());

std::uniform_real_distribution<float> rand_f32(0, 1);
std::uniform_int_distribution<uint32_t> rand_u32(0, UINT32_MAX);

int main(int, char **) {
  ocl_warpper::ocl_resource rcs(1, 0);
  if (!rcs.ok()) {
    cout << rcs.error_code() << " : " << rcs.error_detail() << endl;
    return 1;
  }

  cout << "Vendor = " << rcs.device_vendor() << endl;

  const size_t colorset_size = 1 << 14;
  const size_t task_size = 1 << 15;

  const SCL_convertAlgo algo = SCL_convertAlgo::RGB;

  {
    std::vector<float> colorset_R(colorset_size), colorset_B(colorset_size),
        colorset_G(colorset_size);
    for (size_t cid = 0; cid < colorset_size; cid++) {
      colorset_R[cid] = rand_f32(mt);
      colorset_G[cid] = rand_f32(mt);
      colorset_B[cid] = rand_f32(mt);
    }

    // set colorset
    rcs.set_colorset(colorset_size,
                     {colorset_R.data(), colorset_G.data(), colorset_B.data()});
    if (!rcs.ok()) {
      cout << rcs.error_code() << " : " << rcs.error_detail() << endl;
      return 3;
    }
  }
  cout << "colorset set." << endl;
  {
    // initialize task with random colors
    std::vector<uint32_t> tasks(task_size);
    for (size_t tid = 0; tid < task_size; tid++) {
      tasks[tid] = rand_u32(mt) | 0xFF'00'00'00;
    }
    // set tasks
    rcs.set_task(tasks.data(), tasks.size());
    if (!rcs.ok()) {
      cout << rcs.error_code() << " : " << rcs.error_detail() << endl;
      return 2;
    }
  }

  cout << "tasks set." << endl;

  rcs.execute(algo);
  if (!rcs.ok()) {
    cout << rcs.error_code() << " : " << rcs.error_detail() << endl;
    return 4;
  }

  cout << "Success" << endl;
  return 0;
}