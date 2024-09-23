import { Injectable } from '@angular/core';
import { Album } from '../model/Album';

@Injectable({
  providedIn: 'root',
})
export class AlbumService {
  readonly baseUrl = 'https://angular.io/assets/images/tutorials/faa';

  protected housingLocationList: Album[] = [
    
  ];

  getAllAlbums(): Album[] {
    return this.housingLocationList;
  }

  getHousingLocationById(id: number): Album | undefined {
    return this.housingLocationList.find(
      (housingLocation) => housingLocation.id === id
    );
  }
}
