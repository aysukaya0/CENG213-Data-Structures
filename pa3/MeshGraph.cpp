#include "MeshGraph.h"
#include "BinaryHeap.h"

// For printing
#include <fstream>
#include <iostream>
#include <sstream>

MeshGraph::MeshGraph(const std::vector<Double3>& vertexPositions,
                     const std::vector<IdPair>& edges)
{
    // TODO:
    int verticesSize = vertexPositions.size();
    vertices.resize(verticesSize);
    for (int i=0; i<verticesSize; i++){
        vertices[i].position3D = vertexPositions[i];
        vertices[i].id = i;
    }
    adjList.resize(verticesSize);
    for (int i=0; i<edges.size(); i++){
        adjList[edges[i].vertexId0].push_front(&vertices[edges[i].vertexId1]);
        adjList[edges[i].vertexId1].push_front(&vertices[edges[i].vertexId0]);
    }
}
    
double MeshGraph::AverageDistanceBetweenVertices() const
{
    // TODO:
    double result=0;
    int number=0;
    for (int i=0; i<adjList.size(); i++){
        for (std::list<Vertex*>::const_iterator it = adjList[i].begin(); it != adjList[i].end(); it++){
            number++;
            result += Double3::Distance(vertices[i].position3D, (*it)->position3D);
        }
    }
    return result/number;
}

double MeshGraph::AverageEdgePerVertex() const
{
    // TODO:
    double result=0;
    int number=adjList.size();
    for (int i=0; i<number; i++){
        for (std::list<Vertex*>::const_iterator it = adjList[i].begin(); it != adjList[i].end(); it++){
            result++;
        }
    }
    return result/(2*number);
}

int MeshGraph::TotalVertexCount() const
{
    // TODO:
    return vertices.size();
}

int MeshGraph::TotalEdgeCount() const
{
    // TODO:
    int result=0;
    for (int i=0; i<adjList.size(); i++){
        for (std::list<Vertex*>::const_iterator it = adjList[i].begin(); it != adjList[i].end(); it++){
            result++;
        }
    }
    return result/2;
}

int MeshGraph::VertexEdgeCount(int vertexId) const
{
    // TODO:
    if (vertexId<0 || vertices.size() <= vertexId) return -1;
    int result=0;
    for (std::list<Vertex*>::const_iterator it = adjList[vertexId].begin(); it != adjList[vertexId].end(); it++){
            result++;
    }
    return result;
}

void MeshGraph::ImmediateNeighbours(std::vector<int>& outVertexIds,
                                    int vertexId) const
{
    // TODO:
    if (vertexId<0 || vertices.size() <= vertexId) return;
    for (std::list<Vertex*>::const_iterator it = adjList[vertexId].begin(); it != adjList[vertexId].end(); it++){
            outVertexIds.push_back((*it)->id);
    }
}

void MeshGraph::PaintInBetweenVertex(std::vector<Color>& outputColorAllVertex,
                                     int vertexIdFrom, int vertexIdTo,
                                     const Color& color) const
{
    // TODO:
    outputColorAllVertex.resize(0);
    if (vertexIdFrom<0 || vertexIdFrom>=vertices.size() || vertexIdTo<0 || vertexIdTo>=vertices.size()) return;
    outputColorAllVertex.resize(vertices.size());
    for (int i=0; i<outputColorAllVertex.size(); i++){
        outputColorAllVertex[i].r = 0;
        outputColorAllVertex[i].g = 0;
        outputColorAllVertex[i].b = 0;
    }
    std::vector<int> previous;
    previous.resize(vertices.size());
    std::vector<double> distance;
    distance.resize(vertices.size());
    for (int i=0; i<vertices.size(); i++){
        if (i==vertexIdFrom) distance[i] = 0;
        else distance[i] = INFINITY;
    }
    BinaryHeap shortest;
    shortest.Add(vertexIdFrom, 0);
    while (shortest.HeapSize() != 0){
        int Id;
        double weight;
        shortest.PopHeap(Id, weight);
        for (std::list<Vertex*>::const_iterator it = adjList[Id].begin(); it != adjList[Id].end(); it++){
            double newWeight = Double3::Distance(vertices[Id].position3D, (*it)->position3D);
            int adj = (*it)->id;
            if (weight + newWeight < distance[adj]){
                previous[adj] = Id;
                distance[adj] = weight + newWeight;
                if (!shortest.Add(adj, weight + newWeight)){
                    shortest.ChangePriority(adj, weight + newWeight);
                }
            }
        }
    }
    int j = vertexIdTo;
    while (j != vertexIdFrom){
        outputColorAllVertex[j] = color;
        j = previous[j];
    }
    outputColorAllVertex[vertexIdFrom] = color;
}

void MeshGraph::PaintInRangeGeodesic(std::vector<Color>& outputColorAllVertex,
                                    int vertexId, const Color& color,
                                    int maxDepth, FilterType type,
                                    double alpha) const
{
    // TODO:
    outputColorAllVertex.resize(0);
    if (vertexId<0 || vertexId>=vertices.size()) return;
    outputColorAllVertex.resize(vertices.size());
    for (int i=0; i<outputColorAllVertex.size(); i++){
        outputColorAllVertex[i].r = 0;
        outputColorAllVertex[i].g = 0;
        outputColorAllVertex[i].b = 0;
    }
    std::vector<int> depths(vertices.size(),-1);
    depths[vertexId] = 0;
    std::vector<double> weights(vertices.size());
    weights[vertexId] = 0;
    std::vector<bool> visited(vertices.size(), false);
    visited[vertexId] = true;
    BinaryHeap bft;
    bft.Add(vertexId, vertexId);
    int n = 0;
    while (bft.HeapSize() != 0){
        n += vertices.size();
        int Id;
        double weight;
        bft.PopHeap(Id, weight);
        for (std::list<Vertex*>::const_iterator it = adjList[Id].begin(); it != adjList[Id].end(); it++){
            if (!visited[(*it)->id]){
                double newWeight = Double3::Distance(vertices[Id].position3D, (*it)->position3D);
                weights[(*it)->id] = weights[Id] + newWeight;
                visited[(*it)->id] = true;
                depths[(*it)->id] = depths[Id] + 1;
                bft.Add((*it)->id, n+(*it)->id);
            }
        }
    }
    double r = color.r;
    double g = color.g;
    double b = color.b;
    for (int i=0; i<vertices.size(); i++){
        if (i == vertexId){
            outputColorAllVertex[i] = color;
        }
        else if (depths[i] != -1 && depths[i] <= maxDepth){
            if (type == FILTER_GAUSSIAN){
                outputColorAllVertex[i].r = r*filter_gaussian(weights[i], alpha);
                outputColorAllVertex[i].g = g*filter_gaussian(weights[i], alpha);
                outputColorAllVertex[i].b = b*filter_gaussian(weights[i], alpha);
            }
            else {
                outputColorAllVertex[i].r = r*filter_box(weights[i], alpha);
                outputColorAllVertex[i].g = g*filter_box(weights[i], alpha);
                outputColorAllVertex[i].b = b*filter_box(weights[i], alpha);
            }
        }
    }
}

double MeshGraph::filter_gaussian(double x, double alpha) const{
    return std::exp(-(std::pow(x,2))/std::pow(alpha,2));
}

double MeshGraph::filter_box(double x, double alpha) const{
    if (-alpha <= x && x <= alpha) return 1;
    return 0;
}

void MeshGraph::PaintInRangeEuclidian(std::vector<Color>& outputColorAllVertex,
                                      int vertexId, const Color& color,
                                      int maxDepth, FilterType type,
                                      double alpha) const
{
    // TODO:
    outputColorAllVertex.resize(0);
    if (vertexId<0 || vertexId>=vertices.size()) return;
    outputColorAllVertex.resize(vertices.size());
    for (int i=0; i<outputColorAllVertex.size(); i++){
        outputColorAllVertex[i].r = 0;
        outputColorAllVertex[i].g = 0;
        outputColorAllVertex[i].b = 0;
    }
    std::vector<int> depths(vertices.size(),-1);
    depths[vertexId] = 0;
    std::vector<double> weights(vertices.size());
    weights[vertexId] = 0;
    std::vector<bool> visited(vertices.size(), false);
    visited[vertexId] = true;
    BinaryHeap bft;
    bft.Add(vertexId, vertexId);
    int n = 0;
    while (bft.HeapSize() != 0){
        n += vertices.size();
        int Id;
        double weight;
        bft.PopHeap(Id, weight);
        for (std::list<Vertex*>::const_iterator it = adjList[Id].begin(); it != adjList[Id].end(); it++){
            if (!visited[(*it)->id]){
                weights[(*it)->id] = Double3::Distance(vertices[vertexId].position3D, (*it)->position3D);
                visited[(*it)->id] = true;
                depths[(*it)->id] = depths[Id] + 1;
                bft.Add((*it)->id, n+(*it)->id);
            }
        }
    }
    double r = color.r;
    double g = color.g;
    double b = color.b;
    for (int i=0; i<vertices.size(); i++){
        if (i == vertexId){
            outputColorAllVertex[i] = color;
        }
        else if (depths[i] != -1 && depths[i] <= maxDepth){
            if (type == FILTER_GAUSSIAN){
                outputColorAllVertex[i].r = r*filter_gaussian(weights[i], alpha);
                outputColorAllVertex[i].g = g*filter_gaussian(weights[i], alpha);
                outputColorAllVertex[i].b = b*filter_gaussian(weights[i], alpha);
            }
            else {
                outputColorAllVertex[i].r = r*filter_box(weights[i], alpha);
                outputColorAllVertex[i].g = g*filter_box(weights[i], alpha);
                outputColorAllVertex[i].b = b*filter_box(weights[i], alpha);
            }
        }
    }
}

void MeshGraph::WriteColorToFile(const std::vector<Color>& colors,
                                 const std::string& fileName)
{
    // IMPLEMENTED
    std::stringstream s;
    for(int i = 0; i < static_cast<int>(colors.size()); i++)
    {
        int r = static_cast<int>(colors[i].r);
        int g = static_cast<int>(colors[i].g);
        int b = static_cast<int>(colors[i].b);

        s << r << ", " << g << ", " << b << "\n";
    }
    std::ofstream f(fileName.c_str());
    f << s.str();
}

void MeshGraph::PrintColorToStdOut(const std::vector<Color>& colors)
{
    // IMPLEMENTED
    for(int i = 0; i < static_cast<int>(colors.size()); i++)
    {
        std::cout << static_cast<int>(colors[i].r) << ", "
                  << static_cast<int>(colors[i].g) << ", "
                  << static_cast<int>(colors[i].b) << "\n";
    }
}