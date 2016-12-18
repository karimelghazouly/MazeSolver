#include <iostream>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/opencv.hpp"
using namespace cv;
using namespace std;
Mat image=imread("maze2.jpg",1);//reading the maze globally so i can use it in functions easily
struct cord
{
    //making a struct for cordinate
    int x;
    int y;
};
cord parent[10000][10000];
int visit[10000][10000];
cord findFirst()
{
    cord f;
    for(int i=0;i<image.rows;i++)
    {
        for(int j=0;j<image.cols;j++)
        {
            if(image.at<Vec3b>(i,j)[0]<=25&&image.at<Vec3b>(i,j)[1]<=25&&image.at<Vec3b>(i,j)[2]<=25)//searching for the first column of black pixel and first row of black pixel
            {
                f.x=j;
                f.y=i;
                return f;
            }
        }
    }
}
void draw(int pr,int pc,int firstr,int firstc)
{
    int r,c;
    while(1)
    {
        image.at<Vec3b>(pr,pc)[0]=0;
        image.at<Vec3b>(pr,pc)[1]=255;
        image.at<Vec3b>(pr,pc)[2]=0;
        image.at<Vec3b>(pr,pc+1)[0]=0;
        image.at<Vec3b>(pr,pc+1)[1]=255;
        image.at<Vec3b>(pr,pc+1)[2]=0;
        image.at<Vec3b>(pr,pc+2)[0]=0;
        image.at<Vec3b>(pr,pc+2)[1]=255;
        image.at<Vec3b>(pr,pc+2)[2]=0;
        if(pr==firstr&&pc==firstc)break;
        r=pr;
        c=pc;
        pr=parent[r][c].x;
        pc=parent[r][c].y;
    }
}
int main()
{

    bool done=false;
    int startRow,startCol,currentr,currentc,dr,dc;
    startCol=findFirst().x; //First coloumn of black pixels to avoid white borders
    startRow=findFirst().y; //First row of black pixels to aboid white borders
    for(int i=startCol+2;i<image.cols;i++)
    {
        if(image.at<Vec3b>(startRow,i)[0]>=225&&image.at<Vec3b>(startRow,i)[1]&&image.at<Vec3b>(startRow,i)[2]>=225)//searching for the maze entrance
        {
                startCol=i+1;
                break;
        }
    }
    queue< pair<int,int> > q;
    q.push(make_pair(startRow,startCol));
    //BFS + Shortest Path
    while(!q.empty())
    {
        currentr=q.front().first;//taking info from queue
        currentc=q.front().second;//taking info from queue
        //cout<<"current r = "<<currentr<<" current c = "<<currentc<<endl;
        if(currentr==image.rows-1)// if we are at the finish line of the maze
        {
            dr=currentr;
            dc=currentc;
            done=true;
        }
        if(currentr-1>=0&&currentr-1>=startRow&&image.at<Vec3b>(currentr-1,currentc)[0]>=225&&image.at<Vec3b>(currentr-1,currentc)[1]>=225&&image.at<Vec3b>(currentr-1,currentc)[2]>=225&&visit[currentr-1][currentc]==0)//checking if pixel is white before adding it to the queue
        {
            q.push(make_pair(currentr-1,currentc));
            parent[currentr-1][currentc].x=currentr;//saving parents of pixel
            parent[currentr-1][currentc].y=currentc;//saving parents of pixel
            visit[currentr-1][currentc]=1;//mark pixel as visited
        }
        if(currentr+1<image.rows&&image.at<Vec3b>(currentr+1,currentc)[0]>=225&&image.at<Vec3b>(currentr+1,currentc)[1]>=225&&image.at<Vec3b>(currentr+1,currentc)[2]>=225&&visit[currentr+1][currentc]==0)//checking if pixel is white before adding it to the queue
        {
            q.push(make_pair(currentr+1,currentc));
            parent[currentr+1][currentc].x=currentr;//saving parents of pixel
            parent[currentr+1][currentc].y=currentc;//saving parents of pixel
            visit[currentr+1][currentc]=1;//mark pixel as visited

        }
        if(currentc-1>=0&&image.at<Vec3b>(currentr,currentc-1)[0]>=225&&image.at<Vec3b>(currentr,currentc-1)[1]>=225&&image.at<Vec3b>(currentr,currentc-1)[2]>=225&&visit[currentr][currentc-1]==0)//checking if pixel is white before adding it to the queue
        {
            q.push(make_pair(currentr,currentc-1));
            parent[currentr][currentc-1].x=currentr;//saving parents of pixel
            parent[currentr][currentc-1].y=currentc;//saving parents of pixel
            visit[currentr][currentc-1]=1;//mark pixel as visited
        }
        if(currentr<image.cols&&image.at<Vec3b>(currentr,currentc+1)[0]>=225&&image.at<Vec3b>(currentr,currentc+1)[1]>=225&&image.at<Vec3b>(currentr,currentc+1)[2]>=225&&visit[currentr][currentc+1]==0)//checking if pixel is white before adding it to the queue
        {
            q.push(make_pair(currentr,currentc+1));
            parent[currentr][currentc+1].x=currentr;//saving parents of pixel
            parent[currentr][currentc+1].y=currentc;//saving parents of pixel
            visit[currentr][currentc+1]=1;//mark pixel as visited
        }
        q.pop();

    }
    draw(dr,dc,startRow,startCol);//drawing the path found
    namedWindow("image",CV_WINDOW_FREERATIO);//making window to show the solved maze
    imshow("image",image);
    waitKey(0);

    return 0;
}
