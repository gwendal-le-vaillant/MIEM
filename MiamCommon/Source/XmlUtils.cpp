/*
 ==============================================================================
 
 XmlUtils.cpp
 Created: 12 Jun 2017 9:36:59am
 Author:  Gwendal Le Vaillant
 
 ==============================================================================
 */

#include "XmlUtils.h"

#include <vector>
#include <regex>

using namespace Miam;

size_t XmlUtils::CheckIndexes(const bptree::ptree& tree, const std::string& parentPath, const std::string& childrenPath, const std::string& indexPath)
{
    std::vector< unsigned int > indexesCounts;
    for (auto& child : tree.get_child(parentPath))
    {
        // tag name check
        if (child.first == childrenPath)
        {
            // indexes counts check
            size_t indexFound;
            try {
                indexFound = child.second.get<size_t>(indexPath);
            }
            catch(bptree::ptree_error &e) {
                throw XmlReadException("<" + childrenPath + "> is missing a <" + indexPath + " child: ", e);
            }
            // resize() n'efface aucun élément si on agrandit le vecteur
            if (indexFound >= indexesCounts.size())
                indexesCounts.resize(indexFound+1, 0); // pushing back zeros
            (indexesCounts[indexFound])++;
        }
        for (size_t i=0 ; i<indexesCounts.size() ; i++)
            if (indexesCounts[i] != 1)
                throw XmlReadException("Each <" + childrenPath + "> child index attribute (named '" + indexPath + "') must be found exactly 1 time ( here, index values within [ 0 ; " + std::to_string(indexesCounts.size()-1) + " ] )");
    }
    
    return indexesCounts.size();
}


bool XmlUtils::IsIpv4AddressValid(const std::string& ipAddressToCheck)
{
    bool isIpValid = true;
    // Regex pour retrouver les composantes de l'adresse IP,
    // qu'on vérifiera 1 par 1 juste après
    std::regex get2IpComponentsRegex("(.*?)\\.(.*?)");
    std::smatch matchResults;
    std::string remainingIpToCheck = ipAddressToCheck;
    int ipComponentsCount = 0;
    // While we find ip components (sub strings)
    while ( std::regex_search(remainingIpToCheck, matchResults, get2IpComponentsRegex)
        && isIpValid
        && ipComponentsCount < 4)
    {
        // Check for validity
        isIpValid = isBetween_0_255( *( ++(matchResults.begin()) ) );
        // Next step
        if (isIpValid)
        {
            ipComponentsCount++;
            remainingIpToCheck = matchResults.suffix();
        }
    }
    // Test of remaining string (last component, alone)
    isIpValid = isBetween_0_255(remainingIpToCheck);
    if (isIpValid)
        ipComponentsCount++;
    // Ip components counting
    if (ipComponentsCount != 4)
        isIpValid = false;
    
    return isIpValid;
}
bool XmlUtils::isBetween_0_255(std::string str)
{
    bool isBetween = true;
    int ipComponent = -1;
    try {
        ipComponent = std::stoi(str);
    }
    catch (std::exception) {isBetween = false;}
    
    // Numeric values check
    if (ipComponent < 0 || 256 <= ipComponent)
        isBetween = false;
    
    return isBetween;
}
