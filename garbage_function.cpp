## function garbage. Just in case...

/*

void vE_clear_all();
void E::vE_clear_all()
{
    for (std::vector<E*>::iterator it_all=this->vE.begin();it_all!=this->vE.end()-1;++it_all)
    {
        if (!this->vE.empty())
        {
            E * pt_to_E = (*it_all);
            delete pt_to_E;
            (*it_all)->vE_clear_all();
            
        }
        else 
        {
            this->vE.clear();
            break;
        }
    }
    this->vE.clear();
}        



*/
