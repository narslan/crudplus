import { Component, inject } from '@angular/core';
import { AlbumsComponent } from './albums.component';
import { Album } from '../../model/Album';
import { CommonModule } from '@angular/common';
import { AlbumService } from '../../service/album.service';

@Component({
  selector: 'app-album-home',
  standalone: true,
  imports: [CommonModule, AlbumsComponent],
  template: `
    <section>
      <form>
        <input type="text" placeholder="Filter by name" />
        <button class="primary" type="button">Search</button>
      </form>
    </section>
    <section class="results">
      <app-albums *ngFor="let album of albumList" [album]="album"></app-albums>
    </section>
  `,
  styles: `
    .results {
  display: grid;
  column-gap: 14px;
  row-gap: 14px;
  grid-template-columns: repeat(auto-fill, minmax(400px, 400px));
  margin-top: 50px;
  justify-content: space-around;
}

input[type="text"] {
  border: solid 1px var(--primary-color);
  padding: 10px;
  border-radius: 8px;
  margin-right: 4px;
  display: inline-block;
  width: 30%;
}

button {
  padding: 10px;
  border: solid 1px var(--primary-color);
  background: var(--primary-color);
  color: white;
  border-radius: 8px;
}
  `,
})
export class AlbumHomeComponent {
  albumList: Album[] = [];
  albumService: AlbumService = inject(AlbumService);
  constructor() {
    this.albumList = this.albumService.getAllAlbums();
  }
}
