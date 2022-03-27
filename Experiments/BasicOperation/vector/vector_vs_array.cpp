#include <cstdlib>
#include <vector>
# include <iostream>
#include <iostream>
#include <string>
using namespace std;
#include <boost/date_time/posix_time/ptime.hpp>
#include <boost/date_time/microsec_time_clock.hpp>

class TestTimer
{
public:
    TestTimer(const std::string & name) : name(name),
                                          start(boost::date_time::microsec_clock<boost::posix_time::ptime>::local_time())
    {
    }

    ~TestTimer()
    {
        using namespace std;
        using namespace boost;

        posix_time::ptime now(date_time::microsec_clock<posix_time::ptime>::local_time());
        posix_time::time_duration d = now - start;

       // cout << name << " completed in " << d.total_milliseconds() / 1000.0 <<
           //  " seconds" << endl;
    }
    int duration() {
        using namespace std;
        using namespace boost;

        posix_time::ptime now(date_time::microsec_clock<posix_time::ptime>::local_time());
        posix_time::time_duration d = now - start;
        return d.total_milliseconds();

    }

private:
    std::string name;
    boost::posix_time::ptime start;
};

struct Pixel
{
    Pixel()
    {
    }

    Pixel(unsigned char r, unsigned char g, unsigned char b) : r(r), g(g), b(b)
    {
    }

    unsigned char r, g, b;
};

struct Integer_
{
    Integer_()
    {
    }

    Integer_(int a) : a(a)
    {
    }

    int a;
};


void UseVector()
{
    double timeread = 0;
    TestTimer t("UseVector");

    for(int i = 0; i < 1000; ++i)
    {
        const int iniNum = 1000000;

        vector<Integer_> vec;
        vec.resize(iniNum);
        for(int i = 0; i < iniNum; ++i)
        {
            vec[i].a = i+1;
        }
       // cout << "use vector, after initialization" << endl;
        TestTimer t1("");
      //  int * qans = new int[iniNum];
      Integer_ * answer = (Integer_*)malloc(sizeof(Integer_) * iniNum);
        int * qans = (int *)malloc(sizeof(int) * iniNum);
        for(int j = 0 ; j < iniNum; ++j)
        {
            answer[j] = vec[j];
        }
        timeread += t1.duration();
        free(qans);
    }
    std::cout << "time read for UseVector = " << timeread << endl;

}

//void UseVectorPushBack()
//{
//    double timeread = 0;
//    TestTimer t("UseVectorPushBack");
//
//    for(int i = 0; i < 1000; ++i)
//    {
//        int dimension = 999;
//
//        std::vector<Pixel> pixels;
//        pixels.reserve(dimension * dimension);
//
//        for(int i = 0; i < dimension * dimension; ++i)
//            pixels.push_back(Pixel(255, 0, 0));
//        TestTimer t1("");
//        Pixel * pixels_read = (Pixel *)malloc(sizeof(Pixel) * dimension * dimension);
//
//        for(int j = 0 ; j < dimension * dimension; ++j)
//        {
//            pixels_read[j] = pixels[j];
//        }
//        timeread += t1.duration();
//    }
//    std::cout << "time read for UseVectorPushBack = " << timeread << endl;
//}

void UseArray()
{
    TestTimer t("UseArray");
    double timeread = 0;
    for(int i = 0; i < 1000; ++i)
    {
        const int iniNum = 1000000;
        Integer_ * standard_array = (Integer_*)malloc(sizeof(Integer_) * iniNum);
        // int * standard_array = (int*) malloc(sizeof(int) * iniNum);
        for (int i = 0; i < iniNum; i ++) {
            standard_array[i].a = i+1;
        }

        TestTimer t1("");
        Integer_ * answer = (Integer_*)malloc(sizeof(Integer_) * iniNum);

        for(int j = 0 ; j < iniNum; ++j)
        {
            answer[j] = standard_array[j];
        }
        timeread += t1.duration();
        free(answer);
    }
    std::cout << "time read for UseArray = " << timeread << endl;

}

int main()
{
    TestTimer t1("The whole thing");

    UseArray();
    UseVector();
    // UseVectorPushBack();

    return 0;
}