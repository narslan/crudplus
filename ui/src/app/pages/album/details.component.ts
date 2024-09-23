import { Component, inject } from '@angular/core';
import { ActivatedRoute } from '@angular/router';
import { AlbumService } from '../../service/album.service';
import { Album } from '../../model/Album';

@Component({
  selector: 'app-details',
  standalone: true,
  imports: [],
  template: `
    <article>
      <img class="listing-photo" [src]="album?.photo" />
      <section class="listing-description">
        <h2 class="listing-heading">
          {{ album?.name }}
        </h2>
        <p class="listing-location">{{ album?.city }} {{ album?.state }}</p>
      </section>
      <section class="listing-features">
        <h2 class="section-headers">Ürün özellikleri</h2>
        <ul>
          <li>Units available: {{ album?.availableUnits }}</li>
          <li>Wifi: {{ album?.wifi }}</li>
          <li>Çamaşır Makinesi {{ album?.laundry }}</li>
        </ul>
      </section>
    </article>
  `,
  styles: `
 
  `,
})
export class DetailsComponent {
  route: ActivatedRoute = inject(ActivatedRoute);
  albumService: AlbumService = inject(AlbumService);
  album: Album | undefined;
  constructor() {
    const albumId = Number(this.route.snapshot.params['id']);
    this.album = this.albumService.getHousingLocationById(albumId);
  }
}
