#include <iostream>
#include <string>

namespace UUID{

#include "PA2.c"

unsigned int CHARTOHEX(char _in){
	switch(_in)
	{
		case '0': return 0; break;
		case '1': return 1; break;
		case '2': return 2; break;
		case '3': return 3; break;
		case '4': return 4; break;
		case '5': return 5; break;
		case '6': return 6; break;
		case '7': return 7; break;
		case '8': return 8; break;
		case '9': return 9; break;
		case 'a': return 10; break;
		case 'b': return 11; break;
		case 'c': return 12; break;
		case 'd': return 13; break;
		case 'e': return 14; break;
		case 'f': return 15; break;
		case 'A': return 10; break;
		case 'B': return 11; break;
		case 'C': return 12; break;
		case 'D': return 13; break;
		case 'E': return 14; break;
		case 'F': return 15; break;
	}
	std::cerr << "Unrecognized character. returning 0" << std::endl;
	return 0;
}

unsigned int GET32BITS(std::string &input){
	
	
		unsigned int temp = 0;
		temp = CHARTOHEX(input[0]);
		 temp *= 0x10; input.erase(0,1);
		temp += CHARTOHEX(input[0]);
		 temp *= 0x10; input.erase(0,1);
		temp += CHARTOHEX(input[0]);
		 temp *= 0x10; input.erase(0,1);
		temp += CHARTOHEX(input[0]);
		 input.erase(0,1);
		
		return temp;
	
}

class UUIDclass{
  private:
	
	UUID id;
	
  public:
	UUIDclass(){
		FILLUUID(&id);
		NORMALIZEUUID(&id);
		STAMPUUID(&id);
	}
	
	UUIDclass(std::string input){
		/* id :: top_top,
				 top_high_mid,
				 top_low_mid,
				 top_bottom,
				 bottom_top,
				 bottom_high_mid,
				 bottom_low_mid,
				 bottom_bottom;
		 example uuid:
		   //84bd f796 - 66cc - 4655 - 9b89 - d621 8d10 0f9c
		 */
		 try {
			
			id.top_top = GET32BITS(input);
			id.top_high_mid = GET32BITS(input);
			
			if(input[0] == '-') input.erase(0,1);
			//          \/
			//84bd f796 - 66cc - 4655 - 9b89 - d621 8d10 0f9c
		 
			id.top_low_mid = GET32BITS(input);
			
			if(input[0] == '-') input.erase(0,1);
			//                 \/
			//84bd f796 - 66cc - 4655 - 9b89 - d621 8d10 0f9c
			
			id.top_bottom = GET32BITS(input);
			
			if(input[0] == '-') input.erase(0,1);
			//                        \/
			//84bd f796 - 66cc - 4655 - 9b89 - d621 8d10 0f9c
		 
			id.bottom_top = GET32BITS(input);
			
			if(input[0] == '-') input.erase(0,1);
			//                               \/
			//84bd f796 - 66cc - 4655 - 9b89 - d621 8d10 0f9c
			
			id.bottom_high_mid = GET32BITS(input);
			id.bottom_low_mid = GET32BITS(input);
			id.bottom_bottom = GET32BITS(input);
			
			//convert into UUID format
			NORMALIZEUUID(&id);
			
			//verify UUID
			if(this->getVersion() == 0){
				std::cerr << "Input UUID has invalid version." << std::endl;
			}
			if(this->getVariant() == 4){
				std::cerr << "Input UUID has invalid variant." << std::endl;				
			}
		}
		catch(...){
			std::cerr << "An error has occured with input UUID." << std::endl;
		}
			
	}
	
	void display(void){
		DISPLAYUUID(&id);
	}

	UUID getUUID(void) const{
		return id;
	}
	
	int getVersion(void) const{
		if(this->getVariant() !=1)
		{
			//std::cerr << "Version unknown by this program (not rfc4122) " << std::endl;
			//return 0;
		}
		unsigned int version;
		version = (id.time_high_and_version & 0xf000);
		switch(version){
			case 0x1000:
				return 1;
				break;
			case 0x2000:
				return 2;
				break;
			case 0x3000:
				return 3;
				break;
			case 0x4000:
				return 4;
				break;
			case 0x5000:
				return 5;
				break;
		}
		return 0;
	}
	
	int getVariant(void) const{
		unsigned short variant;
		// 1110 0000 = 0xe0
		variant = (id.clock_seq_and_reserved & 0xe0);
		//111x xxxx : Reserved for future definition.
		if(variant == 0xe0) return 3;
		// 1100 0000 = 0xc0
		variant &= 0xc0;
		//110x xxxx : Reserved, Microsoft Corporation backward
		if(variant == 0xc0) return 2; //compatibility
		// 1000 000 = 0x80
		variant &= 0x80;
		//10xx xxxx : The variant specified in this document.
		if(variant == 0x80)	return 1; //(http://www.ietf.org/rfc/rfc4122.txt)
		//0xxx xxxx : Reserved, NCS backward compatibility.
		if(variant == 0x00)	return 0;
		
		std::cerr << "Unknown variant. " << std::endl;
		return 4;
		
	}

	bool displayVerVar(void)
	{
		switch(this->getVariant())
		{
			case 0:
				std::cout << "Variant: Reserved, NCS backward compatibility. ";
				std::cout << "Unknown Version" << std::endl;
			break;

			case 1:
				std::cout << "Variant: RFC 4122, ";
				std::cout << "Version: " << this->getVersion() << std::endl;
			break;

			case 2:
				std::cout << "Variant: Reserved Microsoft Corporation backward compatibility. ";
				std::cout << "Unknown Version" << std::endl;
			break;

			case 3:
				std::cout << "Variant:Reserved for future definition. ";
				std::cout << "Unknown Version" << std::endl;
			break;

			case 4:
				// std::cout << "Uknown Variant ";
				std::cout << "Unknown Version" << std::endl;
			break;
		}
		return true;
	}
	
	bool operator== (const UUIDclass& RHS){
		UUID RHSid = RHS.getUUID();
		/**
		 	unsigned int time_low:32; 				//4hexOctets
			unsigned int time_mid:16;				//2hexOctets
			unsigned int time_high_and_version:16;	//2hexOctets
			unsigned int clock_seq_and_reserved:8;	//1hexOctets
			unsigned int clock_seq_low:8;			//1hexOctets
			unsigned int node_high:16;				//2 of 6hexOctets
			unsigned int node_low:32;				//4 of 6hexOctets
		 * /
		*/
		if(id.time_low != RHSid.time_low) return false;
		if(id.time_mid != RHSid.time_mid) return false;
		if(id.time_high_and_version != RHSid.time_high_and_version) return false;
		if(id.clock_seq_and_reserved != RHSid.clock_seq_and_reserved) return false;
		if(id.clock_seq_low != RHSid.clock_seq_low) return false;
		if(id.node_high != RHSid.node_high) return false;
		if(id.node_low != RHSid.node_low) return false;
		
		return true;
		
	}
	
};

};

using namespace UUID;

int main(int argc, char* argv[])
{   
	SEEDRANDOM();

	std::cout << "Joshua Underwood: PA3 - C++" << std::endl
		 <<"CSCI 3415: fall 2012" << std::endl
		 <<"DUE October 17, 2012" << std::endl
		 << std::endl
		 << "Generated UUID's:" << std::endl;
	
	UUIDclass *uuid;
//	UUIDclass testing;
	
	for(int i = 1; i<=10; ++i)
	{
		uuid = new UUIDclass();
		UUIDclass temp;
		temp = (*uuid);
		std::cout << " " << i << "\t";
		uuid->display();
		uuid->displayVerVar();
		delete uuid;
	}
	
	std::cout << std::endl
			  << "Entered UUID's from assignment:" 
			  << std::endl << std::endl;
	
	std::cout << " 1\t";
	uuid = new UUIDclass("ab725f00-0ca6-11e2-9303-80c16e4418b1");
	uuid->display();
	uuid->displayVerVar();
	delete uuid;

	std::cout << " 2\t";
	uuid = new UUIDclass("3d813cbb-47fb-32ba-91df-831e1593ac29");
	uuid->display();
	uuid->displayVerVar();
	delete uuid;

	std::cout << " 3\t";
	uuid = new UUIDclass("21f7f8de-8051-5b89-8680-0195ef798b6a");
	uuid->display();
	uuid->displayVerVar();
	delete uuid;

	std::cout << " 4\t";
	uuid = new UUIDclass("84bdf796-66cc-4655-9b89-d6218d100f9c");
	uuid->display();
	uuid->displayVerVar();
	delete uuid;

	std::cout << " 5\t";
	uuid = new UUIDclass("6ba7b810-9dad-11d1-80b4-00c04fd430c8");
	uuid->display();
	uuid->displayVerVar();
	delete uuid;

	
   
   //system("PAUSE");
	return 0;
}	

