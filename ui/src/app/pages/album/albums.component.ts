import { Component, Input } from '@angular/core';
import { Album } from '../../model/Album';
import { RouterModule } from '@angular/router';

@Component({
  selector: 'app-albums',
  standalone: true,
  imports: [RouterModule],
  template: ` <section class="listing">
    <img
      class="listing-photo"
      [src]="album.photo"
      alt="Exterior photo of {{ album.name }}"
    />
    <h2 class="listing-heading">{{ album.name }}</h2>
    <p class="listing-location">{{ album.city }}, {{ album.state }}</p>
    <a [routerLink]="['/details', album.id]"> Learn More</a>
  </section>`,
  styles: `

`,
})
export class AlbumsComponent {
  @Input() album!: Album;
}
