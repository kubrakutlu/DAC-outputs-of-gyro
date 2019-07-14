#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <math.h>

using namespace std;

double dac0510(double g){
    double  v;
    //DAC specifications
    double min_vol=0.0;
    double max_vol=5.0;
    double resolution=10.0;
    //Calculation of values that need to be sent to DAC
    v = ( (pow(2.0,resolution)-1.0) / (max_vol-min_vol) ) * g;
    //cout<<v<<endl;
    return v;
}

double dac1014(double g){
    double  v;
    // DAC specifications
    double min_vol=0.0;
    double max_vol=10.0;
    double resolution=14.0;
    //Calculation of values that need to be sent to DAC
    v = ( (pow(2.0,resolution)-1.0) / (max_vol-min_vol) ) * g;
    //cout<<v<<endl;
    return v;
}


int main(int argc, char* argv[])
{
    ifstream input(argv[1]);
    ifstream config(argv[3]);
    ofstream output;
    output.open(argv[2]);

    string str_input;
    string str_config;
    string V_str;
    stringstream ss;
    vector<string> config_contents;
    vector<double> voltage;
    double sensitivity[6]={0.125,0.5,0.5,0.0025,0.0125,0.05};
    double V[6]={};
    int l;
    //int t=0;
    double x,product;
    size_t pos;

    //Check config file
    if(config.is_open() && config.good())
    {
        cout << "Configuration file is opened."<<"\n"<<"Context:"<<"\n";
        //Read config.txt
        while (getline(config, str_config))
        {
        cout<<str_config<<endl;
        config_contents.push_back(str_config);
        }
        //Read input.txt and process
        while(getline(input, str_input))
        {
            str_input.erase(0,2); //Delete the number of line value in the beginning
            str_input.append("\t");
            //cout<<str_input<<endl;
            l=0;
                while ( (pos=str_input.find("\t")) != string::npos ) //Seperate x y z axes data
                {
                    //cout<<str_input.substr(0, pos);
                    stringstream ss(str_input.substr(0, pos));
                    str_input.erase(0, pos + 1);

                    ss>>x;
                    product=x*sensitivity[l];
                    //cout<<product<<endl;
                    voltage.push_back(product);
                    //cout<<voltage[l]<<endl;
                    l+=1;

                }
                // Obtain DAC outputs according to DAC models
                for(int i=0; i<config_contents.size(); i++)
                {
                    if(config_contents[i].compare("DAC0510") == 0){
                        if(voltage.size()==config_contents.size()){
                            V[i]=dac0510(voltage[i]);}
                        else{
                            V[i]=dac0510(voltage[i+(voltage.size()-config_contents.size())]);}
                    }

                    else if(config_contents[i].compare("DAC1014") == 0){
                        if(voltage.size()==config_contents.size())
                            V[i]=dac1014(voltage[i]);
                        else
                            V[i]=dac1014(voltage[i+(voltage.size()-config_contents.size())]);
                    }
                }

                //Convert integer DAC outputs to string
                for(int k=0; k<config_contents.size(); k++)
                {
                    stringstream ss2;
                    //cout<<V[k]<<endl;
                    ss2<<V[k];
                    V_str.append(ss2.str());
                    V_str += "  ";
                    //cout<<V_str;
                }
            V_str += '\n';
        }
        //cout<<V_str<<"\n";

        //Write into output.txt
        output<<V_str;
        output.close();
    }
    else
        cout << "Configuration file could not be opened.";
    return 0;
}
