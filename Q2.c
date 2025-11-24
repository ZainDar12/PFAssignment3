#include <stdio.h>
#include <math.h>
int calculateFuel(int fuel,int consumption,int recharge,int SolarBonus,int planet,int totalPlanets,int callNo){
    if(fuel<0) return 0;
    if(planet>totalPlanets)return 1;
    fuel-=consumption;
    // assume every 5th planet gives gravitational assist
    fuel+=callNo%4==0?SolarBonus:0;
    fuel+=callNo%5==0?recharge:0;
    printf("Planet %d: Fuel Remaining = %d\n", planet, fuel);
    return calculateFuel(fuel,consumption,recharge,SolarBonus,planet+1,totalPlanets,callNo+1);

}
int main{
    int success = calculateFuel(1000, 90, 50, 30, 1, 10);
    if (success) printf("Journey completed!\n");
    else printf("Ran out of fuel!\n");
}
