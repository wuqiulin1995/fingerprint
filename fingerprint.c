/*
 * Initializes an array that obeys the Zipf distribution
 * @ Pzipf: Zipf distribution array
 * @ fingerprint: an array that maps fingerprints to physical page number (PPNs)
 * @ UNIQUE_PAGE_NB: the number of data pages with unique content
 */
void INIT_zipf_AND_fingerprint(struct ssdstate *ssd)
{
	int i;
	double a = 0.2, sum = 0.0;

	ssd->Pzipf = (double*)calloc(UNIQUE_PAGE_NB+1, sizeof(double));
	ssd->fingerprint = (int64_t*)calloc(UNIQUE_PAGE_NB+1, sizeof(int64_t));

	for(i = 0; i <= UNIQUE_PAGE_NB; i++)
	{
		ssd->Pzipf[i] = 0.0;
		ssd->fingerprint[i] = -1;
	}

	for(i = 1; i <= UNIQUE_PAGE_NB; i++)
	{
		sum += 1 / pow((double)i, a);
	}

	for(i = 1; i <= UNIQUE_PAGE_NB; i++)
	{
		ssd->Pzipf[i] = ssd->Pzipf[i-1] + 1 / pow((double)i, a) / sum;
	}
}

/*
 * Acquire a fingerprint for each incoming LPN by using the Pzipf array
 * @ fp: fingerprint
 * @ dup_ppn: find the PPN from the fingerprint array, 
 *   dup_ppn = -1 means the LPN is a new data, we write the data to a new flash page and update the fingerprint array
 *   dup_ppn >= 0 means the LPN is a duplicate data, we map the LPN to the dup_ppn
 */

fp = 0;
data = ((double)rand()+1)/((double)RAND_MAX+2);
low = 0;
high = UNIQUE_PAGE_NB;
while (low < high)
{
    mid = low + (high-low+1)/2;

    if (data <= ssd->Pzipf[mid]) 
    {
        if (data > ssd->Pzipf[mid-1])
        {
            fp = mid;
            break;
        }

        high = mid-1;
    } 
    else 
    {
        low = mid;
    }
}

dup_ppn = ssd->fingerprint[fp];
