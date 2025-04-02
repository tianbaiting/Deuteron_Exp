#include <iostream>


double get_ratio(std::string pols , std::string target, std::string gamma){
    if (pols == "z_pol") {

        std::string folder = "../input_tree/z_pol/b_continuous/d+" + target + "E190g" +
                    gams[i] + p + "/";
        std::string filename = string{"dbreakb"} + ".dat";
        std::cout << folder + filename << std::endl;
        std::ifstream inputfile{folder + filename};
        return R;
    }else if (pols == "y_pol")
    {
        std::string folder = "../input_tree/y_pol/phi_fixed/d+" + target + "E190g" +
                    gams[i] + p + "/";
        std::string filename = string{"dbreakb"} + ".dat";
        std::cout << folder + filename << std::endl;
        std::ifstream inputfile{folder + filename};
        return R;
    }
    

}
