#include "decoder.h"


namespace esphome {
namespace nikai {


bool NikaiDecoder::decode(
    remote_base::RemoteReceiveData &data,
    uint32_t *result
)
{

    // стартовый импульс
    if (!data.expect_item(4000, 4000))
        return false;



    uint32_t value = 0;



    for (int i = 0; i < 24; i++)
    {

        if (!data.expect_mark(500))
            return false;



        if (data.expect_space(2000))
        {

            value <<= 1;
            value |= 1;

        }
        else if (data.expect_space(1000))
        {

            value <<= 1;

        }
        else
        {
            return false;
        }

    }


    *result = value;


    return true;

}


}
}
